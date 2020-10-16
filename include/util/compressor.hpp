#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>

namespace Decima {
    enum class CompressionLevel {
        None,
        SuperFast,
        VeryFast,
        Fast,
        Normal,
        Optimal1,
        Optimal2,
        Optimal3,
        Optimal4,
        Optimal5
    };

    class Compressor {
    public:
        typedef int (*CompressFn)(int format, const std::uint8_t* src, std::size_t src_len, std::uint8_t* dst, int level, void*, std::size_t, std::size_t, void*, std::size_t);
        typedef int (*DecompressFn)(const std::uint8_t* src, std::size_t src_len, std::uint8_t* dst, std::size_t dst_len, int fuzz, int crc, int verbose, std::uint8_t*, std::size_t, void*, void*, void*, std::size_t, int);

    public:
        explicit inline Compressor(HMODULE module)
            : m_module(module)
            , m_compress(reinterpret_cast<CompressFn>(GetProcAddress(m_module, "OodleLZ_Compress")))
            , m_decompress(reinterpret_cast<DecompressFn>(GetProcAddress(m_module, "OodleLZ_Decompress"))) { }

        explicit inline Compressor(const std::string& path)
            : Compressor(LoadLibraryA(path.c_str())) { }

        explicit inline Compressor(const std::wstring& path)
            : Compressor(LoadLibraryW(path.c_str())) { }

        inline ~Compressor() {
            FreeLibrary(m_module);
        }

        template <typename Input, typename Output>
        inline int compress(const Input& input, Output& output, CompressionLevel level = CompressionLevel::Normal) const noexcept {
            output.resize(calculate_compression_bound(input.size()));
            return m_compress(8, (std::uint8_t*)input.data(), input.size(), (std::uint8_t*)output.data(), static_cast<int>(level), nullptr, 0, 0, nullptr, 0);
        }

        template <typename Input, typename Output>
        inline int decompress(const Input& input, Output& output, int crc = 0) const noexcept {
            return m_decompress((std::uint8_t*)input.data(), input.size(), (std::uint8_t*)output.data(), output.size(), 0, crc, 0, nullptr, 0, nullptr, nullptr, nullptr, 0, 0);
        }

    private:
        inline static std::size_t calculate_compression_bound(std::size_t size) noexcept {
            return size + 274 * ((size + 0x3FFFF) / 0x40000);
        }

        HMODULE m_module;
        CompressFn m_compress;
        DecompressFn m_decompress;
    };
}
