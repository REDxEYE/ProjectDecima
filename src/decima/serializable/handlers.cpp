#include "decima/serializable/handlers.hpp"

#include "decima/serializable/object/object.hpp"
#include "decima/serializable/object/object_dummy.hpp"
#include "decima/serializable/object/collection.hpp"
#include "decima/serializable/object/translation.hpp"
#include "decima/serializable/object/prefetch.hpp"
#include "decima/serializable/object/texture.hpp"
#include "decima/serializable/object/texture_set.hpp"

class FileMagics {
};

class DeathStranding_FileMagics : public FileMagics {
public:
    // clang-format off
    static constexpr uint64_t Armature    = 0x11e1d1a40b933e66;
    static constexpr uint64_t Texture     = 0xa664164d69fd2b38;
    static constexpr uint64_t TextureSet  = 0xa321e8c307328d2e;
    static constexpr uint64_t Translation = 0x31be502435317445;
    static constexpr uint64_t Shader      = 0x16bb69a9e5aa0d9e;
    static constexpr uint64_t Collection  = 0xf3586131b4f18516;
    static constexpr uint64_t Prefetch    = 0xd05789eae3acbf02;
    // clang-format on
};

class ZeroDawn_FileMagics : public FileMagics {
public:
    static constexpr uint64_t Texture = 0xf2e1afb7052b3866;
};

template <class T, typename... Args>
inline static std::shared_ptr<Decima::CoreObject> construct(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

static const std::unordered_map<std::uint64_t, Decima::Handler<Decima::CoreObject>> handlers {
    // clang-format off
    { DeathStranding_FileMagics::Translation, construct<Decima::Translation> },
    { DeathStranding_FileMagics::Texture,     construct<Decima::Texture>     },
    { DeathStranding_FileMagics::TextureSet,  construct<Decima::TextureSet>  },
    { DeathStranding_FileMagics::Collection,  construct<Decima::Collection>  },
    { DeathStranding_FileMagics::Prefetch,    construct<Decima::Prefetch>    },
    // clang-format on
};

static const std::unordered_map<uint64_t, std::string> names = {
    // clang-format off
    { DeathStranding_FileMagics::Armature,    "Armature"    },
    { DeathStranding_FileMagics::Texture,     "Texture"     },
    { DeathStranding_FileMagics::TextureSet,  "TextureSet"  },
    { DeathStranding_FileMagics::Translation, "Translation" },
    { DeathStranding_FileMagics::Shader,      "Shader"      },
    { DeathStranding_FileMagics::Collection,  "Collection"  },
    { DeathStranding_FileMagics::Prefetch,    "Prefetch"    },
    // clang-format on
};

std::invoke_result_t<Decima::Handler<Decima::CoreObject>> Decima::get_type_handler(std::uint64_t hash) {
    const auto handler = handlers.find(hash);
    return handler != handlers.end() ? handler->second() : construct<Decima::Dummy>();
}

std::string Decima::get_type_name(uint64_t hash) {
    const auto name = names.find(hash);
    return name != names.end() ? name->second : "Unknown '" + uint64_to_hex(hash) + "'";
}