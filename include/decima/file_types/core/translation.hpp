#pragma once

#include <decima/core_file.hpp>
#include "entry.hpp"

namespace Decima {
    class Translation : public CoreEntry {
    public:
        static constexpr const char* languages[] {
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

        std::string translations[std::size(languages)];

        /*
         * Not sure is this is a comment or something else.
         * Let be comment for now.
         */
        std::string comments[std::size(languages)];
        std::uint8_t flags[std::size(languages)];

        void parse(ArchiveArray& archives, ash::buffer& buffer, CoreFile* core_file) override;
        void draw() override;
    };
}
