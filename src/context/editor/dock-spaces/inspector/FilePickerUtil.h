#ifndef FILEPICKERUTIL_H
#define FILEPICKERUTIL_H
#include <nfd.hpp>
#include <iostream>

namespace Metal {
    class FilePickerUtil {
    public:
        static std::string selectAudioFile() {
            NFD::UniquePath outPath;

            std::vector<nfdfilteritem_t> filterList = {
                {"WAV Audio", "wav"},
                {"MP3 Audio", "mp3"},
                {"OGG Vorbis Audio", "ogg"},
                {"FLAC Audio", "flac"},
                {"AAC Audio", "aac"},
                {"M4A Audio", "m4a"},
                {"All Supported Audio", "wav,mp3,ogg,flac,aac,m4a"}, // Combined filter
                {"All Files", "*"} // Optional: Allow any file
            };


            nfdresult_t result = NFD::OpenDialog(outPath, filterList.data(), filterList.size(), nullptr);

            if (result == NFD_OKAY) {
                std::cout << "Success! Selected file: " << outPath.get() << std::endl;
                return std::string(outPath.get()); // Return the selected path
            } else if (result == NFD_CANCEL) {
                std::cout << "User cancelled the dialog." << std::endl;
                return ""; // Return empty string on cancellation
            } else {
                std::cerr << "Error opening file dialog: " << NFD::GetError() << std::endl;
                return ""; // Return empty string on error
            }
        }
    };
}

#endif
