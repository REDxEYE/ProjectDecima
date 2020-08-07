//
// Created by MED45 on 30.07.2020.
//

#include <iomanip>

#include "decima/file_types/core.h"


namespace Decima {
    std::ostream& operator<<(std::ostream& os, GUID guid) {
        return os << std::hex << std::setfill('0')
                  << std::setw(8) << (guid.data[0] >> 32) << '-'
                  << std::setw(4) << (guid.data[0] >> 16 & 0xffff) << '-'
                  << std::setw(4) << (guid.data[0] >> 0 & 0xffff) << '-'
                  << std::setw(4) << (guid.data[1] >> 48) << '-'
                  << std::setw(12) << (guid.data[1] >> 0);
    }

    void CoreFile::parse(std::vector<uint8_t>& buffer) {
        imemstream imembuffer(buffer);
        parse(imembuffer);
        //        parse();
    }

    void CoreFile::parse(std::istream& stream) {
        stream.read(reinterpret_cast<char*>(&header), sizeof(header));
        stream.read(reinterpret_cast<char*>(&guid), 16);
    }

    uint64_t CoreFile::peek_header(std::vector<uint8_t>& buffer) {
        imemstream imembuffer(buffer);
        return peek_header(imembuffer);
    }

    uint64_t CoreFile::peek_header(std::istream& stream) {
        uint64_t magic;
        stream.read(reinterpret_cast<char*>(&magic), 8);
        stream.seekg(-8, std::ios::cur);
        return magic;
    }

}
