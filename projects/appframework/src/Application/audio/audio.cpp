
#include "afpch.h"
#include "assertion.h"

#include "audio_file_loader.h"
#include "audio_debug.h"
#include "audio.h"

namespace af {
	enum class AudioFormat {
		None = 0,
		WAV
	};

	static AudioFormat GetFileFormat(std::string_view file_path) {
		std::filesystem::path path = file_path;
		std::string extension = path.extension().string();
		if (extension == ".wav")
			return AudioFormat::WAV;

		return AudioFormat::None;
	}

	static ALenum GetOpenALFormat_FromWaveHeader(WaveHeader& wave_header) {
		ALenum format = AL_NONE;

		if (wave_header.num_channels == 1 && wave_header.bits_per_sample == 8)       format = AL_FORMAT_MONO8;
		else if (wave_header.num_channels == 1 && wave_header.bits_per_sample == 16) format = AL_FORMAT_MONO16;
		else if (wave_header.num_channels == 2 && wave_header.bits_per_sample == 8)  format = AL_FORMAT_STEREO8;
		else if (wave_header.num_channels == 2 && wave_header.bits_per_sample == 16) format = AL_FORMAT_STEREO16;

		else {
			AF_ERROR(
				"Unknown WAV format detected. Channels: {}, Bits per sample: {}"
				"Ensure the file is a proper WAV and has not been converted from any another audio format!\n",
				wave_header.num_channels,
				wave_header.bits_per_sample
			);
			format = AL_NONE;
		}

		return format;
	}

	Audio::Audio(ALuint audio_buffer_id, ALuint audio_src_id) {
		m_AudioBufferID = audio_buffer_id;
		m_AudioSourceID = audio_src_id;
	}

	Audio::~Audio() {
		if (m_AudioSourceID)
			alCall(alDeleteSources(1, &m_AudioSourceID));

		if (m_AudioBufferID)
			alCall(alDeleteBuffers(1, &m_AudioBufferID));
	}

	Audio Audio::LoadAudio(std::string_view sound_path) {
		AudioFormat audio_file_format = GetFileFormat(sound_path);

		switch (audio_file_format) {
			case AudioFormat::WAV: return LoadAudioDataFromWaveFile(sound_path);
		}

		AF_ASSERT(false, "Unsupported audio format: {}", sound_path);
		return {0, 0};
	}

	void Audio::DestroyAudio(ALCcontext* context) {
		if (context)
			alcMakeContextCurrent(context);

		if (m_AudioSourceID)
			alCall(alDeleteSources(1, &m_AudioSourceID));

		if (m_AudioBufferID)
			alCall(alDeleteBuffers(1, &m_AudioBufferID));
	}

	void Audio::SetSrc() {
		alCall(alSourcei(m_AudioSourceID, AL_BUFFER, m_AudioBufferID));
	}

	void Audio::Play() {
		ALint src_state = AL_NONE;
		alCall(alGetSourcei(m_AudioSourceID, AL_SOURCE_STATE, &src_state));

		if (src_state != AL_PLAYING) {
			alCall(alSourcePlay(m_AudioSourceID));
			AF_WARN("Played!");
		}
	}

	Audio Audio::LoadAudioDataFromWaveFile(std::string_view sound_path) {
		WaveHeader wave_audio_header_data;
		std::vector<char> wave_raw_audio_data;
		bool load_wave = af::AudioLoader::LoadWave(sound_path, &wave_audio_header_data, wave_raw_audio_data);
		AF_ASSERT(load_wave != false, "af::AudioLoader::LoadWave Call Failed! for: {}", sound_path);

		ALuint audio_buffer;
		alCall(alGenBuffers(1, &audio_buffer));
		alCall(alBufferData(
			audio_buffer,
			GetOpenALFormat_FromWaveHeader(wave_audio_header_data),
			wave_raw_audio_data.data(), wave_raw_audio_data.size(),
			wave_audio_header_data.sample_rate
		));
		wave_raw_audio_data.clear();

		ALuint source_handle;
		alCall(alGenSources(1, &source_handle));

		Audio gen_src = { audio_buffer, source_handle };
		return gen_src;
	}

}