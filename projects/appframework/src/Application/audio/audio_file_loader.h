#pragma once

namespace af {
	struct WaveHeader {
		// RIFF
		char chunk_id[4];              //  4  riff_mark[4];
		uint32_t chunk_size;           //  4  file_size;
		char format[4];                //  4  wave_str[4];

		// fmt sub-chunk
		char sub_chunk1_id[4];         //  4  fmt_str[4];
		uint32_t sub_chunk1_size;      //  4  pcm_bit_num;
		uint16_t audio_format;         //  2  pcm_encode;
		uint16_t num_channels;         //  2  sound_channel;
		uint32_t sample_rate;          //  4  pcm_sample_freq;
		uint32_t byte_rate;            //  4  byte_freq;
		uint16_t block_align;          //  2  block_align;
		uint16_t bits_per_sample;      //  2  sample_bits;

		// data sub-chunk
		char sub_chunk2_id[4];         //  4  data_str[4];
		uint32_t sub_chunk2_size;      //  4  sound_size;
	};

	class AudioLoader {
	public:
		static bool LoadWave(std::string_view audio_path, WaveHeader* wave_header_data, std::vector<char>& raw_pcm_bytes);
	};
}