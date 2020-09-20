#pragma once

#include <type_traits>
#include <vector>

#include "decima/serializable/object/object.hpp"
#include "decima/serializable/serializable.hpp"

namespace Decima {
    class ArchiveManager;

    template <typename T, typename = void>
    class Array : public CoreSerializable {
    public:
        static_assert(std::is_trivial_v<T>, "Array type must be either trivial or inherit Decima::CoreSerializable");

        inline void parse(ash::buffer& buffer) {
            std::uint32_t length = buffer.get<decltype(length)>();
            m_data.resize(length);
            buffer.get(m_data);
        }

        inline const std::vector<T>& data() const { return m_data; }
        inline std::vector<T>& data() { return m_data; }

    private:
        std::vector<T> m_data;
    };

    template <typename T>
    class Array<T, std::enable_if_t<std::is_base_of_v<CoreSerializable, T>>> : public CoreSerializable {
    public:
        inline void parse(ash::buffer& buffer) {
            std::uint32_t length = buffer.get<decltype(length)>();
            m_data.resize(length);
            for (auto& item : m_data)
                item.parse(buffer);
        }

        inline const std::vector<T>& data() const { return m_data; }
        inline std::vector<T>& data() { return m_data; }

    private:
        std::vector<T> m_data;
    };

    template <typename T>
    class Array<T, std::enable_if_t<std::is_base_of_v<CoreObject, T>>> : public CoreSerializable {
    public:
        inline void parse(ArchiveManager& manager, ash::buffer& buffer) {
            std::uint32_t length = buffer.get<decltype(length)>();
            m_data.resize(length);
            for (auto& item : m_data)
                item.parse(manager, buffer);
        }

        inline const std::vector<T>& data() const { return m_data; }
        inline std::vector<T>& data() { return m_data; }

    private:
        std::vector<T> m_data;
    };
}