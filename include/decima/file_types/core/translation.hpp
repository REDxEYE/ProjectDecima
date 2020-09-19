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
            "Danish",
            "Finnish",
            "Norwegian",
            "Swedish",
            "Japanese",
            "Spanish (Mexico)",
            "Portuguese (Brazil)",
            "Turkish",
            "Arabic",
            "Chinese (Simplified)",
            "English (UK)",
            "Greek",
            "Czech",
            "Hungarian"
        };

        std::string translations[std::size(languages)];
        std::string comments[std::size(languages)];
        std::uint8_t flags[std::size(languages)];

        void parse(ArchiveArray& archives, ash::buffer& buffer, CoreFile* core_file) override;
        void draw() override;
    };
}
