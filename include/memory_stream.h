//
// Created by MED45 on 26.07.2020.
//

#ifndef PROJECTDS_MEMORY_STREAM_H
#define PROJECTDS_MEMORY_STREAM_H

#include <streambuf>
#include <utility>
#include <vector>
#include <fstream>

class MemoryStream : public std::streambuf {
public:
    std::vector<uint8_t> memory;

    explicit MemoryStream(std::vector<uint8_t> mem) : memory(std::move(mem)) {
        setg(reinterpret_cast<char*>(memory.data()), reinterpret_cast<char*>(memory.data()),
             reinterpret_cast<char*>(memory.data() + memory.size()));
        setp(reinterpret_cast<char*>(memory.data()), reinterpret_cast<char*>(memory.data() + memory.size()));
    }

    virtual pos_type
    seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which = std::ios_base::in) {
        if (dir == std::ios_base::cur) gbump(off);
        return gptr() - eback();
    }

    std::istream as_stream() {
        return std::istream(this);
    }

};

#endif //PROJECTDS_MEMORY_STREAM_H
