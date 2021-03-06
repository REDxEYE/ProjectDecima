#pragma once

#include <functional>

#include "decima/shared.hpp"
#include "decima/archive/archive_file.hpp"
#include "decima/serializable/serializable.hpp"
#include "util/buffer.hpp"

namespace Decima {
    using StringMutator = std::function<std::string(const std::string&)>;

    class String : public CoreSerializable {
    public:
        void parse(ash::buffer& buffer, CoreFile& file);
        void draw();
        void draw(StringMutator mutator);

        inline const std::string& data() const noexcept { return m_data; }

    protected:
        std::string m_data;
    };

    class StringHashed : public CoreSerializable {
    public:
        void parse(ash::buffer& buffer, CoreFile& file);
        void draw();
        void draw(StringMutator mutator);

        inline std::uint32_t hash() const noexcept { return m_hash; }
        inline const std::string& data() const noexcept { return m_data; }

    private:
        std::uint32_t m_hash;
        std::string m_data;
    };
}
