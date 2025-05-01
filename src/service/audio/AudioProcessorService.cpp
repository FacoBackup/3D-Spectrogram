#include "AudioProcessorService.h"

#include "STFTUtil.h"
#include "../../context/ApplicationContext.h"
#include "../../service/voxel/impl/SparseVoxelOctreeBuilder.h"
#include "../buffer/BufferInstance.h"

namespace Metal {
    AudioDataVector AudioProcessorService::readAudioData() const {
        std::string filePath = context.editorRepository.pathToAudio;
        AudioDataVector audioData;
        SF_INFO sfinfo;
        SNDFILE *infile = nullptr;

        memset(&sfinfo, 0, sizeof(sfinfo));

        infile = sf_open(filePath.c_str(), SFM_READ, &sfinfo);
        if (!infile) {
            std::string errorMsg = "Could not open audio file '" + filePath + "': " + sf_strerror(nullptr);
            sf_close(infile);
            context.notificationRepository.addNotification(errorMsg);
            return audioData;
        }

        if (sfinfo.frames == 0) {
            sf_close(infile);
            context.notificationRepository.addNotification("Audio file contains no frames or is invalid: " + filePath);
            return audioData;
        }
        if (sfinfo.samplerate == 0) {
            sf_close(infile);
            context.notificationRepository.addNotification("Audio file has an invalid sample rate (0): " + filePath);
            return audioData;
        }
        if (sfinfo.channels == 0) {
            sf_close(infile);
            context.notificationRepository.addNotification("Audio file has an invalid channel count (0): " + filePath);
            return audioData;
        }

        constexpr size_t BUFFER_FRAMES = 4096;
        size_t bufferSize = BUFFER_FRAMES * sfinfo.channels;
        std::vector<double> buffer(bufferSize);
        sf_count_t framesRead = 0;
        sf_count_t totalFramesRead = 0;

        audioData.data.reserve(sfinfo.frames);

        bool breakWhile = false;
        while ((framesRead = sf_readf_double(infile, buffer.data(), BUFFER_FRAMES)) > 0 && !breakWhile) {
            for (sf_count_t i = 0; i < framesRead; ++i) {
                auto timestamp = static_cast<double>(totalFramesRead + i) / sfinfo.samplerate;
                if (timestamp < context.editorRepository.rangeStart) {
                    continue;
                }
                if (timestamp > context.editorRepository.rangeEnd) {
                    breakWhile = true;
                    break;
                }
                AudioDataPoint point{};
                point.timestamp = timestamp;
                point.amplitude = buffer[i * sfinfo.channels];
                audioData.data.push_back(point);
            }
            totalFramesRead += framesRead;
        }

        int errorNum = sf_error(infile);
        if (errorNum != SF_ERR_NO_ERROR) {
            std::string errorMsg = "Error reading audio file: " + std::string(sf_error_number(errorNum));
            context.notificationRepository.addNotification(errorMsg);
            sf_close(infile);
            return audioData;
        }

        if (sf_close(infile) != 0) {
            context.notificationRepository.addNotification("Não foi possível fechar o arquivo de audio", WARNING);
        }
        context.notificationRepository.addNotification("Audio lido com sucesso", SUCCESS);
        return audioData;
    }

    void AudioProcessorService::extractAudioData() const {
        if (!context.editorRepository.pathToAudio.empty()) {
            SF_INFO info = {};
            SNDFILE *snd = sf_open(context.editorRepository.pathToAudio.c_str(), SFM_READ, &info);
            if (!snd) {
                context.editorRepository.pathToAudio = "";
                context.notificationRepository.addNotification("Não foi possível obter informações do arquivo");
            }

            context.editorRepository.sampleRate = info.samplerate;
            context.editorRepository.channels = info.channels;
            context.editorRepository.frames = info.frames;
            context.editorRepository.selectedAudioSize = static_cast<double>(info.frames) / info.samplerate;

            if (static_cast<int>(context.editorRepository.selectedAudioSize) < context.editorRepository.sampleSize) {
                context.editorRepository.pathToAudio = "";
                context.notificationRepository.addNotification(
                    "O tamanho audio é menor que " + std::to_string(context.editorRepository.sampleSize) + " segundos");
            }

            sf_close(snd);
        }
    }
}
