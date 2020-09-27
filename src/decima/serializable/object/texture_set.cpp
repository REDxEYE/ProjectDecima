#include "decima/serializable/object/texture_set.hpp"

void Decima::TextureDefaultColor::parse(ash::buffer& buffer, CoreFile& file) {
    buffer.get(rgba, sizeof(rgba));
}

void Decima::DecimaTextureSetEntry::parse(ash::buffer& buffer, CoreFile& file) {
    compression_method = buffer.get<decltype(compression_method)>();
    create_mip_maps = buffer.get<decltype(create_mip_maps)>();
    color_space = buffer.get<decltype(color_space)>();
    packing_info = buffer.get<decltype(packing_info)>();
    texture_type = buffer.get<decltype(texture_type)>();
    texture.parse(buffer, file);
}

void Decima::DecimaTextureSetTextureDescriptor::parse(ash::buffer& buffer, CoreFile& file) {
    texture_type = buffer.get<decltype(texture_type)>();
    path.parse(buffer, file);
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
    default_color.parse(buffer, file);
}

void Decima::TextureSet::parse(ArchiveManager& manager, ash::buffer& buffer, CoreFile& file) {
    CoreObject::parse(manager, buffer, file);

    entries.resize(buffer.get<std::uint32_t>());
    for (auto& entry : entries)
        entry.parse(buffer, file);

    mip_map_mode = buffer.get<decltype(mip_map_mode)>();

    descriptors.resize(buffer.get<std::uint32_t>());
    for (auto& entry : descriptors)
        entry.parse(buffer, file);

    preset.parse(buffer, file);
}
