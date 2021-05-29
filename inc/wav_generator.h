/********************************************************** *
*                                                           *
*                                                           *
*    wav_generator.h                                        *
*                                                           *
*    By: Alvaro Romero <alvromero96@gmail.com>              *
*                                                           *
*    Created: 2021/05/29                                    *
*                                                           *
* **********************************************************/

#include <iostream>
#include <fstream>
#include <cmath>

// Riff chunk
const std::string chunk_id = "RIFF";
const std::string chunk_size = "----";
const std::string format = "WAVE";

// FMT sub-chunk
const std::string subchunk1_id = "fmt ";
const int subchunk1_size = 16;
const int audio_format = 1;
const int num_channels = 2;
const int sample_rate = 44100;
const int byte_rate = sample_rate * num_channels * (subchunk1_size / 8);
const int block_align = num_channels * (subchunk1_size / 8);
const int bits_per_sample = 16;

// Data sub-chunk
const std::string subchunk2_id = "data";
const std::string subchunk2_size = "----";

const int duration = 15;
const int max_amplitude = 32760;
const double frequency = 250;

void write_as_bytes(std::ofstream &file, int value, int byte_size);
