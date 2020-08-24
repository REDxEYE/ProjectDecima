//
// Created by i.getsman on 07.08.2020.
//

#ifndef PROJECTDS_FILE_TYPES_HPP
#define PROJECTDS_FILE_TYPES_HPP

#include <map>

#include "decima/file_types/core/entry.h"
#include "decima/file_types/core/collection.hpp"
#include "decima/file_types/core/dummy_entry.h"
#include "decima/file_types/core/translation.hpp"
#include "decima/file_types/core/prefetch.h"
#include "decima/file_types/core/texture.h"
#include "decima/file_types/core/texture_set.h"


template<class Base, typename... Args>
using Constructor = std::function<std::shared_ptr<Base>(Args&& ...)>;

template<class T, typename... Args>
static std::shared_ptr<Decima::CoreEntry> construct(Args&& ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

static const std::map<std::uint64_t, Constructor<Decima::CoreEntry>> types = {
        {Decima::DeathStranding_FileMagics::Translation, construct<Decima::Translation>},
        {Decima::DeathStranding_FileMagics::Texture,     construct<Decima::Texture>},
        {Decima::DeathStranding_FileMagics::TextureSet,  construct<Decima::TextureSet>},
        {Decima::DeathStranding_FileMagics::Collection,  construct<Decima::Collection>}
};

static auto get_handler(std::uint64_t hash) noexcept {
    const auto handler = types.find(hash);
    return handler != types.end() ? handler->second() : construct<Decima::Dummy>();
}

#endif //PROJECTDS_FILE_TYPES_HPP
