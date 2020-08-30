#pragma once

#include "decima/shared.hpp"

#include <functional>

namespace Decima {
    using StringMutator = std::function<std::string(const std::string&)>;

    class String {
    public:
        void parse(ash::buffer& buffer);
        void draw();
        void draw(StringMutator mutator);

        inline const std::string& data() const noexcept { return m_data; }

    protected:
        std::string m_data;
    };

    class StringHashed {
    public:
        void parse(ash::buffer& buffer);
        void draw();
        void draw(StringMutator mutator);

        inline std::uint32_t hash() const noexcept { return m_hash; }
        inline const std::string& data() const noexcept { return m_data; }

    private:
        std::uint32_t m_hash;
        std::string m_data;
    };
}
