
#include "afpch.h"
#include "assertion.h"
#include "audio_debug.h"
#include "audio_device.h"

namespace af {
	bool GetAvailableSoundDevices(std::vector<std::string>& devices_list_ref) {
		devices_list_ref.clear();

		const ALCchar* devices = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
		AF_ASSERT(devices != nullptr, "alcGetString Failed!");
		if (!devices) return false;

		const char* devices_ptr = devices;
		while (*devices_ptr != '\0') {
			devices_list_ref.emplace_back(devices_ptr);
			devices_ptr += std::strlen(devices_ptr) + 1;
		}

		return true;
	}

	static bool s_Initialized = false;

	AudioDevice::AudioDevice(const char* device_name) : m_alcDevice(nullptr), m_alcContext(nullptr), m_CurrentDeviceName("")
	{
		if (SetDevice(device_name)) { // nullptr = default device
			if(SetContext())  // default device context
				s_Initialized = true;
		}
	}

	AudioDevice::~AudioDevice() {
		Shutdown();
	}

	bool AudioDevice::SetDevice(const char* device_name) {
		Shutdown(); // shutdown old device and context if exists1

		m_alcDevice = alcOpenDevice(device_name);
		AF_ASSERT(m_alcDevice != nullptr, "alcOpenDevice Failed!");
		if (!m_alcDevice) return false;

		const ALchar* device_name_str = nullptr;
		if (!device_name) // if the device name is a nullptr then its using the default device 
			device_name_str = alcGetString(m_alcDevice, ALC_DEFAULT_DEVICE_SPECIFIER);
		else
			device_name_str = alcGetString(m_alcDevice, ALC_ALL_DEVICES_SPECIFIER);

		if (!device_name_str)
			m_CurrentDeviceName = "unknown sound device!";
		else
			m_CurrentDeviceName = device_name_str;

		//AF_INFO("Sound device set to : {}", m_CurrentDeviceName);

		return true;
	}

	bool AudioDevice::SetContext() {
		if (m_alcContext) {
			alcMakeContextCurrent(nullptr);
			alcDestroyContext(m_alcContext);
			m_alcContext = nullptr;
			AF_INFO("Destroyed old sound device context!");
		}

		AF_ASSERT(m_alcDevice != nullptr, "sound device* is null");
		if (!m_alcDevice) return false;

		m_alcContext = alcCreateContext(m_alcDevice, nullptr);
		AF_ASSERT(m_alcContext != nullptr, "alcCreateContext Failed!");
		if (!m_alcContext) return false;

		ALCboolean context = alcMakeContextCurrent(m_alcContext);
		AF_ASSERT(context != ALC_FALSE, "alcMakeContextCurrent Failed!");
		if (!context) {
			alcDestroyContext(m_alcContext);
			m_alcContext = nullptr;
			return false;
		}

		//AF_INFO("Sound device context set to sound device: {}", m_CurrentDeviceName);
		return true;
	}

	void AudioDevice::Shutdown() {
		if (m_alcContext) {
			alcMakeContextCurrent(nullptr);
			alcDestroyContext(m_alcContext);
			m_alcContext = nullptr;
		}

		if (m_alcDevice) {
			alcCloseDevice(m_alcDevice);
			m_alcDevice = nullptr;
		}
	}

}