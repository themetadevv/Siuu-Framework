#pragma once

#include "core.h"
#include "maths.h"

#include "AL/al.h"

#include "audio_device.h"

namespace af {
	class AF_API Audio {
	private:
		ALuint m_AudioBufferID;
		ALuint m_AudioSourceID;

		Audio(ALuint audio_buffer_id, ALuint audio_src_id);

	public:
		Audio() : m_AudioBufferID(0), m_AudioSourceID(0) {}
		~Audio();

		static Audio LoadAudio(std::string_view sound_path);

		void DestroyAudio(ALCcontext* device);

		void SetSrc();
		void Play();

	private:
		static Audio LoadAudioDataFromWaveFile(std::string_view sound_path);
	};
}
 