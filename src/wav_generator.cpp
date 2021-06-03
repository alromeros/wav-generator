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

void write_as_bytes(std::fstream &file, int value, int byte_size)
{
    file.write(reinterpret_cast<const char*>(&value), byte_size);
}

int wav_generator(const char *name)
{
    std::cout << "wav-generator!" << std::endl;

    std::fstream wav;
    std::string filename;

    if (!name)
    {
        std::cout << "Name your WAV file: ";
        std::cin >> filename;
        filename.append(".wav");
    }
    else
    {
        filename = name;
    }
    
    try
    {
        wav.open((filename.c_str() ? filename.c_str() : "test.wav"), std::ios::out | std::ios::binary);
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

int wav_modifier()
{
    std::string     filename;
    std::string     filenameCopy;

    std::cout << "wav-modifier!" << std::endl;
    std::cout << "Enter your file's name: ";
    std::cin >> filename;

    std::ifstream file(filename);
    if(!file.good())
    {
        std::cout << "File not found" << std::endl;
        return 1;
    }
    
    const char *fileFormat = strrchr(filename.c_str(), '.');
    if (strcmp(fileFormat, ".wav"))
    {
        std::cout << "Non-valid file" << std::endl;
    }

    filenameCopy.reserve(filename.length() + 5);
    filenameCopy.assign(filename, 0, filename.length() - strlen(fileFormat));
    filenameCopy += "_copy.wav";
    wav_generator(filenameCopy.c_str());

    std::fstream wavFileCopy;
    wavFileCopy.open(filenameCopy, std::ios::in | std::ios::binary);

    if (!wavFileCopy.is_open())
    {
        std::cout << "ERROR: Couldn't open '" << filenameCopy << "'" << std::endl;
        return 1;
    }

    file.seekg(20);
    wavFileCopy.seekg(20);

    int i = 0;
    while (i < sample_rate * duration)
    {
        int currentByte = file.get() + 460;
        wavFileCopy.write(reinterpret_cast<const char*>(&currentByte), 2);
        i++;
    }

    return 0;
}

int main(int argc, char **argv)
{
    int     ret = 0;
    char    option[2048];

    (void)argc;

    std::cout << "Welcome to " << argv[0] << "!" << std::endl;
    std::cout << "What do you want to do?" << std::endl;
    std::cout << "1 - Generate a partially random .wav file, written from scratch" << std::endl;
    std::cout << "2 - Modify a copy of an already existing .wav file" << std::endl;

    int breakFlag = 0;

    do
    {
        std::cout << "Enter your option (1-2): ";
        std::cin >> option;

        switch (atoi(option)) 
        {
            case 1 :
                ret = wav_generator(NULL);
                breakFlag = 1;
                break;
            case 2 :
                ret = wav_modifier();
                breakFlag = 1;
                break;
            default :
                std::cout << "Non valid option, please, try again!" << std::endl;
        }

    } while (!breakFlag);


    return ret;
}

