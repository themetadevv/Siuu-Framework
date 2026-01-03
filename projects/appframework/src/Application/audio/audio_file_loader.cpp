
#include "afpch.h"
#include "assertion.h"

#include "audio_debug.h"
#include "audio_file_loader.h"

namespace af {
	bool AudioLoader::LoadWave(std::string_view audio_path, WaveHeader* wave_header_data, std::vector<char>& raw_pcm_bytes) {
		if (!std::filesystem::exists(audio_path)) {
			AF_ERROR("Failed to load WAV file '{}'. The file does not exists at the specified path!", audio_path);
			return false;
		}

		std::ifstream audio_file(std::string(audio_path), std::ios::binary);

		if (!audio_file.is_open()) {
			AF_ERROR("Failed to open audio file '{}'. path/file may not exist, be corrupted, or lack read permissions!", audio_path);
			return false;
		}

		audio_file.read(reinterpret_cast<char*>(wave_header_data), sizeof(WaveHeader));

		if (std::memcmp(wave_header_data->chunk_id, "RIFF", 4) != 0 || std::memcmp(wave_header_data->format, "WAVE", 4) != 0) {
			AF_ERROR(
				"WAV header validation failed\n"
				"Path: '{}'\n"
				"Expected: RIFF / WAVE identifiers\n",
				audio_path
			);
			return false;
		}

		raw_pcm_bytes.clear();
		raw_pcm_bytes.resize(wave_header_data->sub_chunk2_size);
		audio_file.read(reinterpret_cast<char*>(raw_pcm_bytes.data()), raw_pcm_bytes.size());

		audio_file.close();
		return true;
	}

}

