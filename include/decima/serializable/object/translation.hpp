#pragma once

#include "decima/serializable/object/object.hpp"

namespace Decima {
    class Translation : public CoreObject {
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

        void parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile* core_file) override;
        void draw() override;
    };
}
