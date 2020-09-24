#include "decima/serializable/object/texture_set.hpp"

void Decima::TextureDefaultColor::parse(ash::buffer& buffer) {
    buffer.get(rgba, sizeof(rgba));
}

void Decima::DecimaTextureSetEntry::parse(ash::buffer& buffer) {
    compression_method = buffer.get<decltype(compression_method)>();
    create_mip_maps = buffer.get<decltype(create_mip_maps)>();
    color_space = buffer.get<decltype(color_space)>();
    packing_info = buffer.get<decltype(packing_info)>();
    texture_type = buffer.get<decltype(texture_type)>();
    texture.parse(buffer);
}

void Decima::DecimaTextureSetTextureDescriptor::parse(ash::buffer& buffer) {
    texture_type = buffer.get<decltype(texture_type)>();
    path.parse(buffer);
    active = buffer.get<decltype(active)>();
    gamma_space = buffer.get<decltype(gamma_space)>();
    storage_type = buffer.get<decltype(storage_type)>();
    quality_type = buffer.get<decltype(quality_type)>();
    compression_method = buffer.get<decltype(compression_method)>();
    if (active > 0) {
        width = buffer.get<decltype(width)>();
        height = buffer.get<decltype(height)>();
    } else {
        unk_0 = buffer.get<decltype(unk_0)>();
    }
    default_color.parse(buffer);
}

void Decima::TextureSet::parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile* core_file) {
    CoreObject::parse(manager, buffer, nullptr);

    entries.resize(buffer.get<std::uint32_t>());
    for (auto& entry : entries)
        entry.parse(buffer);

    mip_map_mode = buffer.get<decltype(mip_map_mode)>();

    descriptors.resize(buffer.get<std::uint32_t>());
    for (auto& entry : descriptors)
        entry.parse(buffer);

    preset.parse(buffer);
}
