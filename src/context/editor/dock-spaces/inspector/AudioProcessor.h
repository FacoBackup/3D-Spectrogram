#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include <sndfile.h>
#include "AudioData.h"

namespace Metal {
    struct AudioProcessor {
        static AudioDataVector readAudioData(const std::string &filePath) {
            AudioDataVector audioData;
            SF_INFO sfinfo;
            SNDFILE *infile = nullptr;

            memset(&sfinfo, 0, sizeof(sfinfo));

            infile = sf_open(filePath.c_str(), SFM_READ, &sfinfo);
            if (!infile) {
                std::string errorMsg = "Could not open audio file '" + filePath + "': " + sf_strerror(nullptr);
                sf_close(infile); // Close if partially opened (though unlikely here)
                throw std::runtime_error(errorMsg);
            }

            if (sfinfo.frames == 0) {
                sf_close(infile);
                throw std::runtime_error("Audio file contains no frames or is invalid: " + filePath);
            }
            if (sfinfo.samplerate == 0) {
                sf_close(infile);
                throw std::runtime_error("Audio file has an invalid sample rate (0): " + filePath);
            }
            if (sfinfo.channels == 0) {
                sf_close(infile);
                throw std::runtime_error("Audio file has an invalid channel count (0): " + filePath);
            }

            std::cout << "Opened Audio File: " << filePath << "\n"
                    << "  Sample Rate: " << sfinfo.samplerate << "\n"
                    << "  Channels: " << sfinfo.channels << "\n"
                    << "  Frames: " << sfinfo.frames << "\n"
                    << "  Format: 0x" << std::hex << sfinfo.format << std::dec << std::endl;


            constexpr size_t BUFFER_FRAMES = 4096; // How many frames to read at a time
            size_t bufferSize = BUFFER_FRAMES * sfinfo.channels;
            std::vector<double> buffer(bufferSize);
            sf_count_t framesRead = 0;
            sf_count_t totalFramesRead = 0;

            audioData.reserve(static_cast<size_t>(sfinfo.frames));

            while ((framesRead = sf_readf_double(infile, buffer.data(), BUFFER_FRAMES)) > 0) {
                for (sf_count_t i = 0; i < framesRead; ++i) {
                    AudioDataPoint point;
                    point.timestamp = static_cast<double>(totalFramesRead + i) / sfinfo.samplerate;
                    point.amplitude = buffer[i * sfinfo.channels];
                    audioData.push_back(point);
                }
                totalFramesRead += framesRead;
            }

            int errorNum = sf_error(infile);
            if (errorNum != SF_ERR_NO_ERROR) {
                std::string errorMsg = "Error reading audio file: " + std::string(sf_error_number(errorNum));
                sf_close(infile); // Close before throwing
                throw std::runtime_error(errorMsg);
            }

            if (sf_close(infile) != 0) {
                std::cerr << "Warning: Could not close audio file properly." << std::endl;
            }

            std::cout << "Successfully read " << totalFramesRead << " frames." << std::endl;
            return audioData;
        }

        static double getDuration(const std::string& path) {
            SF_INFO info = {};
            SNDFILE* snd = sf_open(path.c_str(), SFM_READ, &info);
            if (!snd)
                throw std::runtime_error(sf_strerror(nullptr));
            double duration = static_cast<double>(info.frames) / info.samplerate;
            sf_close(snd);
            return duration;
        }

    };
};

#endif //AUDIOPROCESSOR_H
