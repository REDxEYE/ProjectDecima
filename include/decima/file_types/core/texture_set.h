//
// Created by MED45 on 11.08.2020.
//

#ifndef PROJECTDS_TEXTURE_SET_H
#define PROJECTDS_TEXTURE_SET_H

#include <vector>

#include "decima/core_file.h"
#include "decima/file_types/core/entry.h"
#include "decima/file_types/pod/string.hpp"
#include "decima/file_types/pod/guid.hpp"

namespace Decima {
    struct TextureGUIDEntry {
        uint32_t unk1 {};
        uint32_t unk2 {};
        uint32_t unk3 {};
        uint32_t unk4 {};
        uint16_t unk5 {};
        GUID guid {};
        void parse(Source& stream);
    };

    struct SrcEntry {
        uint32_t slot_id {};
        StringHashed src_name;
        uint16_t unk1 {};
        uint32_t unk2[3] {};
        uint32_t unk_pad{};
        uint32_t width {};
        uint32_t height {};
        float unk3[4] {};
        void parse(Source& stream);
    };

    class TextureSet : public CoreEntry {
        CoreHeader header;
        uint32_t file_count;
        std::vector<TextureGUIDEntry> file_entries;
        uint32_t unk1;
        uint32_t src_count;
        std::vector<SrcEntry> src_entries;
        uint8_t unk2;
    public:
        void parse(ArchiveArray& archives, Source& stream, CoreFile* core_file) override;
        void draw() override;
    };
}
#endif //PROJECTDS_TEXTURE_SET_H
