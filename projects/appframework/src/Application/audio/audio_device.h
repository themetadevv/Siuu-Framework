#pragma once

#include "core.h"
#include <AL/alc.h>

#define USE_DEFAULT_SOUND_DEVICE nullptr

namespace af {
	bool AF_API GetAvailableSoundDevices(std::vector<std::string>& devices_list_ref);

	class AF_API AudioDevice {
	private:
		ALCdevice* m_alcDevice;
		ALCcontext* m_alcContext;
		std::string m_CurrentDeviceName;

	public:
		// nullptr = default device
		AudioDevice(const char* device_name);
		~AudioDevice();

	public:
		bool SetDevice(const char* device_name);
		bool SetContext();

		ALCcontext* GetCtx() { return m_alcContext; }

		std::string_view GetCurrentDeviceName() { return m_CurrentDeviceName; }

	private:
		void Shutdown();
	};
}