

#include <iostream>
#include <format>

#include "application/window/window.h"
#include "application/input/input.h"
#include "application/frames_manager.h"
#include "application/logger.h"

#include "application/audio/audio_device.h"
#include "application/audio/audio.h"

const std::string client_name = "Sandbox";

#define CLIENT_TRACE(...)    ::af::Logger::GetClient(client_name)->trace(__VA_ARGS__)
#define CLIENT_INFO(...)     ::af::Logger::GetClient(client_name)->info(__VA_ARGS__)
#define CLIENT_WARN(...)     ::af::Logger::GetClient(client_name)->warn(__VA_ARGS__)
#define CLIENT_ERROR(...)    ::af::Logger::GetClient(client_name)->error(__VA_ARGS__)
#define CLIENT_CRITICAL(...) ::af::Logger::GetClient(client_name)->critical(__VA_ARGS__)

int main() {

	WindowSpecs Specs;
	Specs.Title = "Siuu";
	Specs.Size = { 1600, 900 };
	Specs.Position = { 1600 / 2, 900 / 2 };
	Specs.State = WindowState::Maximized;
	Specs.VidMode = VideoMode::Windowed;
	Specs.VSync = true;

	af::Logger::Init();
	af::Logger::CreateCoreLogger();
	af::Logger::AddClient(client_name);

	Scope<af::Window> window = CreateScope<af::Window>(Specs);
	af::InputManager::Init(window->GetNativeWindowHandle());
	af::FrameManager::Init(window.get());

	std::vector<std::string> devices;
	af::GetAvailableSoundDevices(devices);

	Scope<af::AudioDevice> sound_dev = CreateScope<af::AudioDevice>(USE_DEFAULT_SOUND_DEVICE);

	af::Audio jump = af::Audio::LoadAudio("assets/test.wav");
	af::Audio ring = af::Audio::LoadAudio("assets/ring.wav");

	bool played = false;

	jump.SetSrc();
	ring.SetSrc();

	CLIENT_INFO(sound_dev->GetCurrentDeviceName());

	while (window->IsRunning())
	{
		if (af::InputManager::KeyPressed(KeyCode::Escape))
			window->Shutdown();

		if (af::InputManager::KeyPressed(KeyCode::F11))
			window->SetVideoMode(VideoMode::Fullscreen);

		if (af::InputManager::KeyPressed(KeyCode::F10))
			window->SetVideoMode(VideoMode::Borderless);

		if (af::InputManager::KeyPressed(KeyCode::F9)) {
			window->SetVideoMode(VideoMode::Windowed);
		}

		if (af::InputManager::KeyPressed(KeyCode::F5)) {
			if (sound_dev->GetCurrentDeviceName() != devices[0]) {
				sound_dev->SetDevice(devices[0].c_str());
				sound_dev->SetContext();
				CLIENT_INFO(sound_dev->GetCurrentDeviceName());
			}
		}

		if (!played) {
			jump.Play();
			ring.Play();
			played = true;
		}

		af::InputManager::Update();
		af::FrameManager::Update();
		window->Update();
	}

	af::Logger::DeleteClient(client_name);
}