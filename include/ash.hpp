#pragma once

#include <algorithm>
#include <stdexcept>
#include <vector>
#include <memory>

namespace ash {
    enum class seek_dir {
        beg,
        cur,
        end
    };

    template <class Inner>
    class buffered_source {
    public:
        using value_type = std::uint8_t;
        using pointer_type = const value_type*;
        using iterator_type = pointer_type;

        buffered_source(const Inner& inner, std::size_t capacity) noexcept
            : buf(capacity)
            , beg(inner.data())
            , cur(inner.data())
            , end(inner.data() + inner.size())
            , pos(0)
            , cap(0) { }

        buffered_source(iterator_type begin, iterator_type end, std::size_t capacity) noexcept
            : buf(capacity)
            , beg(begin)
            , cur(begin)
            , end(end)
            , pos(0)
            , cap(0) { }

        template <class Container>
        std::size_t read(Container& container) noexcept {
            if (pos == cap && std::size(container) >= std::size(buf)) {
                discard();
                return read_inner(container);
            }

            fill();

            const std::size_t count = read_inner(container);
            skip(count);
            return count;
        }

        template <class Type, typename = std::enable_if_t<std::is_trivially_constructible_v<Type>>>
        Type read() {
            std::array<value_type, sizeof(Type)> buffer { 0 };
            read_exact(buffer);
            return *reinterpret_cast<Type*>(buffer.data());
        }

        template <class Container>
        void read_exact(Container& container) {
            if (std::size(container) != read(container))
                throw std::runtime_error("Unexpected end of file");
        }

        std::size_t seek(seek_dir direction, ssize_t count) noexcept {
            std::size_t result = seek_inner(direction, count);
            discard();
            return result;
        }

        void skip(std::size_t amount) noexcept {
            pos = std::min(pos + amount, cap);
        }

        [[nodiscard]] bool eof() const noexcept {
            return cur == end;
        }

        [[nodiscard]] std::size_t tell() const noexcept {
            return std::distance(beg, cur);
        }

        [[nodiscard]] std::size_t capacity() const noexcept {
            return buf.size();
        }

        [[nodiscard]] pointer_type data() const noexcept {
            return buf.data();
        }

    private:
        template <class Container>
        std::size_t read_inner(Container& container) noexcept {
            const std::size_t available = std::min(std::size_t(std::distance(cur, end)), std::size(container));
            std::copy(cur, cur + available, std::begin(container));
            cur += available;
            return available;
        }

        std::size_t seek_inner(seek_dir direction, ssize_t count) noexcept {
            switch (direction) {
            case seek_dir::beg:
                cur = std::clamp(beg + count, beg, end);
                break;
            case seek_dir::cur:
                cur = std::clamp(cur + count, beg, end);
                break;
            case seek_dir::end:
                cur = std::clamp(end - count, beg, end);
                break;
            }

            return std::distance(beg, cur);
        }

        void fill() noexcept {
            if (pos >= cap) {
                cap = read_inner(buf);
                cur -= cap;
                pos = 0;
            }
        }

        void discard() noexcept {
            pos = 0;
            cap = 0;
        }

        std::vector<value_type> buf;
        iterator_type beg;
        iterator_type cur;
        iterator_type end;
        std::size_t pos;
        std::size_t cap;
    };

    template <class Inner>
    class buffered_sink;
}