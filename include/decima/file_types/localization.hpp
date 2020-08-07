//
// Created by i.getsman on 06.08.2020.
//

#ifndef PROJECTDS_LOCALIZATION_HPP
#define PROJECTDS_LOCALIZATION_HPP

#include "decima/file_types/core.h"

#include <array>

namespace Decima {
    class Localization : public CoreFile {
    public:
        static constexpr std::array<const char*, 25> languages = {
            "English",
            "French",
            "Spanish",
            "German",
            "Italian",
            "Dutch",
            "Portuguese",
            "Chinese (Traditional)",
            "Korean",
            "Russian",
            "Polish",
            "Norwegian",
            "Finnish",
            "Swedish",
            "Danish",
            "Japanese",
            "Spanish (Mexico)",
            "Portuguese (Brazil)",
            "Turkish",
            "Arabic",
            "Chinese (Simplified)",
            "Unknown",
            "Greek",
            "Czech",
            "Hungarian"
        };

        std::string translations[languages.size()];

        void parse(std::vector<uint8_t>& buffer) override;

        void parse(std::istream& stream) override;

        void draw(ArchiveArray& archive_array) override;
    };

}
#endif //PROJECTDS_LOCALIZATION_HPP
