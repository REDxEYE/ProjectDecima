//
// Created by i.getsman on 05.08.2020.
//

#ifndef PROJECTDS_MEMBUF_HPP
#define PROJECTDS_MEMBUF_HPP

#include <streambuf>
#include <istream>

struct membuf : std::streambuf {
    membuf(char const* base, size_t size) {
        char* p(const_cast<char*>(base));
        this->setg(p, p, p + size);
    }

    virtual pos_type
    seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which = std::ios_base::in) {
        if (dir == std::ios_base::cur) gbump(off);
        return gptr() - eback();
    }
};

struct imemstream : virtual membuf, std::istream {
    imemstream(char const* base, size_t size)
            : membuf(base, size), std::istream(static_cast<std::streambuf*>(this)) {
    }

    explicit imemstream(std::vector<uint8_t>& buffer)
            : membuf(reinterpret_cast<const char*>(buffer.data()), buffer.size()),
              std::istream(static_cast<std::streambuf*>(this)) {

    }


};

#endif //PROJECTDS_MEMBUF_HPP
