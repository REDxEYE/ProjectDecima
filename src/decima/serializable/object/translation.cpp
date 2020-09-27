#include "decima/serializable/object/translation.hpp"

static std::string read_string(ash::buffer& buffer, const std::string& default_value = "<empty>") {
    const auto length = buffer.get<std::uint16_t>();

    if (length > 0) {
        std::string string(length, '\0');
        buffer.get(string);
        return string;
    }

    return default_value;
}

void Decima::Translation::parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile& file) {
    CoreObject::parse(manager, buffer, file);

    for (uint32_t i = 0; i < std::size(languages); i++) {
        translations[i] = read_string(buffer);
        comments[i] = read_string(buffer);
        flags[i] = buffer.get<char>();
    }
}
