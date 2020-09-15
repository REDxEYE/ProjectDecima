#pragma once

#include <map>

#include "decima/file_types/core/entry.hpp"
#include "decima/file_types/core/collection.hpp"
#include "decima/file_types/core/entry_dummy.hpp"
#include "decima/file_types/core/translation.hpp"
#include "decima/file_types/core/prefetch.hpp"
#include "decima/file_types/core/texture.hpp"
#include "decima/file_types/core/texture_set.h"

template <class Base, typename... Args>
using Constructor = std::function<std::shared_ptr<Base>(Args&&...)>;

template <class T, typename... Args>
static std::shared_ptr<Decima::CoreEntry> construct(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

static const std::map<std::uint64_t, Constructor<Decima::CoreEntry>> types = {
    // clang-format off
    { Decima::DeathStranding_FileMagics::Translation, construct<Decima::Translation> },
    { Decima::DeathStranding_FileMagics::Texture,     construct<Decima::Texture>     },
    { Decima::DeathStranding_FileMagics::TextureSet,  construct<Decima::TextureSet>  },
    { Decima::DeathStranding_FileMagics::Collection,  construct<Decima::Collection>  },
    { Decima::DeathStranding_FileMagics::Prefetch,    construct<Decima::Prefetch>    },
    // clang-format on
};

static auto get_handler(std::uint64_t hash) noexcept {
    const auto handler = types.find(hash);
    return handler != types.end() ? handler->second() : construct<Decima::Dummy>();
}
