#pragma once

#include "decima/file_types/pod/pod.hpp"

namespace Decima {
    class String : public POD {
    public:
        void parse(Source& stream);
        void draw();

        inline const std::string& data() const noexcept { return m_data; }

    protected:
        std::string m_data;
    };

    class StringHashed : public POD {
    public:
        void parse(Source& stream);
        void draw();

        inline std::uint64_t hash() const noexcept { return m_hash; }
        inline const std::string& data() const noexcept { return m_data; }

    private:
        std::uint64_t m_hash;
        std::string m_data;
    };
}