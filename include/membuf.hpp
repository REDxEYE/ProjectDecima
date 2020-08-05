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
        this->setp(p, p + size);
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

//    std::ios::pos_type tellg() {
//        te
//    }


};

#endif //PROJECTDS_MEMBUF_HPP
