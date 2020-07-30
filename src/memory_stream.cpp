//
// Created by MED45 on 26.07.2020.
//

#include "memory_stream.h"

MemoryStream::MemoryStream(std::vector<uint8_t> mem) : memory(std::move(mem)) {
    setg(reinterpret_cast<char*>(memory.data()), reinterpret_cast<char*>(memory.data()),
         reinterpret_cast<char*>(memory.data() + memory.size()));
    setp(reinterpret_cast<char*>(memory.data()), reinterpret_cast<char*>(memory.data() + memory.size()));
}

std::fpos<mbstate_t> MemoryStream::seekoff(long long int off, std::ios_base::seekdir dir, std::ios_base::openmode which) {
    if (dir == std::ios_base::cur) gbump(off);
    return gptr() - eback();
}
