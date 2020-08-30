#pragma once

#include <stdexcept>
#include <string>

namespace ash {
    namespace detail {
        template <class T, typename = void>
        struct is_container : std::false_type { };

        template <class T>
        struct is_container<T, std::void_t<
            typename T::value_type,
            decltype(std::declval<T>().data()),
            decltype(std::declval<T>().size())
        >> : std::true_type { };

        template <class T>
        inline constexpr bool is_container_v = is_container<T>::value;
    }

    template <class CharT, class Traits = std::char_traits<CharT>>
    class basic_buffer {
    public:
        using pointer = const typename Traits::char_type*;

        inline basic_buffer(pointer begin, pointer end) noexcept
            : m_beg(begin)
            , m_end(end) { }

        inline basic_buffer(pointer begin, std::size_t size) noexcept
            : m_beg(begin)
            , m_end(begin + size) { }

        inline basic_buffer(basic_buffer&& other) noexcept = default;
        inline basic_buffer(const basic_buffer& other) noexcept = default;

        basic_buffer& operator=(const basic_buffer& other) noexcept = default;
        basic_buffer& operator=(basic_buffer&& other) noexcept = default;

        inline pointer data() const noexcept { return m_beg; }

        inline pointer begin() const noexcept { return m_beg; }
        inline pointer cbegin() const noexcept { return m_beg; }

        inline pointer end() const noexcept { return m_end; }
        inline pointer cend() const noexcept { return m_end; }

        inline std::size_t size() const noexcept { return std::distance(m_beg, m_end); }

        inline basic_buffer slice(std::size_t offset, std::size_t count) const {
            if (offset + count > size())
                throw std::range_error("Cannot take slice that is larger than buffer");
            return { m_beg + offset, m_beg + offset + count };
        }

        inline basic_buffer take(std::size_t count) const {
            return slice(0, count);
        }

        inline basic_buffer last(std::size_t count) const {
            return slice(size() - count, count);
        }

        inline basic_buffer skip(std::size_t count) const {
            return slice(count, size() - count);
        }

        inline basic_buffer<CharT, Traits>& get(void* dst, std::size_t size) {
            const auto slice = take(size);
            memcpy(dst, slice.data(), slice.size());
            return *this = skip(size);
        }

        template <class Container, typename = std::enable_if_t<detail::is_container_v<Container>>>
        inline void get(Container& container) {
            using Type = typename Container::value_type;
            get(container.data(), container.size() * sizeof(Type));
        }

        template <class Type, typename = std::enable_if_t<std::is_trivial_v<Type>>>
        inline Type get() {
            Type result {};
            get(&result, sizeof(Type));
            return result;
        }

    private:
        pointer m_beg;
        pointer m_end;
    };

    typedef basic_buffer<char> buffer;
}
