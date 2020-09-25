#include "decima/serializable/handlers.hpp"

#include "decima/serializable/object/object.hpp"
#include "decima/serializable/object/object_dummy.hpp"
#include "decima/serializable/object/collection.hpp"
#include "decima/serializable/object/translation.hpp"
#include "decima/serializable/object/prefetch.hpp"
#include "decima/serializable/object/texture.hpp"
#include "decima/serializable/object/texture_set.hpp"

template <class T, typename... Args>
inline static std::shared_ptr<Decima::CoreObject> construct(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

static const std::unordered_map<std::uint64_t, Decima::Handler<Decima::CoreObject>> handlers {
    // clang-format off
    { Decima::DeathStranding_FileMagics::Translation, construct<Decima::Translation> },
    { Decima::DeathStranding_FileMagics::Texture,     construct<Decima::Texture>     },
    { Decima::DeathStranding_FileMagics::TextureSet,  construct<Decima::TextureSet>  },
    { Decima::DeathStranding_FileMagics::Collection,  construct<Decima::Collection>  },
    { Decima::DeathStranding_FileMagics::Prefetch,    construct<Decima::Prefetch>    },
    // clang-format on
};

std::invoke_result_t<Decima::Handler<Decima::CoreObject>> Decima::get_handler(std::uint64_t hash) {
    const auto handler = handlers.find(hash);
    return handler != handlers.end() ? handler->second() : construct<Decima::Dummy>();
}