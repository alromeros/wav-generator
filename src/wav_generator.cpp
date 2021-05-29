/********************************************************** *
*                                                           *
*                                                           *
*    wav_generator.cpp                                      *
*                                                           *
*    By: Alvaro Romero <alvromero96@gmail.com>              *
*                                                           *
*    Created: 2021/05/29                                    *
*                                                           *
* **********************************************************/

#include "wav_generator.h"

void write_as_bytes(std::ofstream &file, int value, int byte_size)
{
    file.write(reinterpret_cast<const char*>(&value), byte_size);
}

int main()
{
    std::ofstream wav;
    std::string filename;

    std::cout << "Name your WAV file: ";
    std::cin >> filename;

    filename.append(".wav");
    
    try
    {
        wav.open((filename.c_str() ? filename.c_str() : "test.wav"), std::ios::binary);
    }
    catch(std::exception &e)
    {
        std::cout << "Error while creating the WAV file: " << e.what() << std::endl;
        return 1;
    }

    if (!wav.is_open())
    {
        std::cout << "Error while opening the WAV file" << std::endl;
        return 1;
    }

    wav << chunk_id;
    wav << chunk_size;
    wav << format;

    wav << subchunk1_id;
    write_as_bytes(wav, subchunk1_size, 4);
    write_as_bytes(wav, audio_format, 2);
    write_as_bytes(wav, num_channels, 2);
    write_as_bytes(wav, sample_rate, 4);
    write_as_bytes(wav, byte_rate, 4);
    write_as_bytes(wav, block_align, 2);
    write_as_bytes(wav, bits_per_sample, 2);

    wav << subchunk2_id;
    wav << subchunk2_size;

    int start_audio = wav.tellp();
    int toneModifier = 16;

    for (int i = 0; i < sample_rate * duration; i++)
    {
        srand(time(NULL));
        // Respect max amplitude
        // operate as a wave
        double amplitude = (double)i / sample_rate * max_amplitude;
        double value = sin((2 * 3.14 * i * frequency / toneModifier) / sample_rate);

        double channel1 = amplitude * value;
        double channel2 = (max_amplitude - amplitude) * value - rand();
    
        write_as_bytes(wav, channel1, 2);
        write_as_bytes(wav, channel2, 2);
        
        if (!(i % 100000) && (toneModifier > 1))
        {
            toneModifier--;
        }
    }

    int end_audio = wav.tellp();

    wav.seekp(start_audio - 4);
    write_as_bytes(wav, end_audio - start_audio, 4);

    wav.seekp(4, std::ios::beg);
    write_as_bytes(wav, end_audio - 8, 4);

    wav.close();
    return 0;
}

