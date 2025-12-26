

#include <iostream>

#include "Application/Window.h"
#include "Application/Input.h"

int main() {
	
	WindowSpecs Specs;
	Specs.Title = "Siuu";
	Specs.Size = { 1600, 900 };
	Specs.Position = { 1600 / 2, 900 / 2 };
	Specs.State = WindowState::Default;
	Specs.VidMode = VideoMode::Windowed;
	Specs.VSync = true;

	Application::Window* window = new Application::Window(Specs);
	Application::InputManager::Init(static_cast<GLFWwindow*>(window->GetNativeWindowHandle()));

	while (window->IsRunning())
	{
		if (Application::InputManager::KeyPressed(KeyCode::F11)) {
			window->SetVideoMode(VideoMode::Fullscreen);
		}
		
		if (Application::InputManager::KeyPressed(KeyCode::F10)) {
			window->SetVideoMode(VideoMode::Borderless);
		}

		if (Application::InputManager::KeyPressed(KeyCode::F9)) {
			window->SetVideoMode(VideoMode::Windowed);
		}

		window->Update();
		Application::InputManager::Update();
	}

	delete window;

	std::cin.get();
}