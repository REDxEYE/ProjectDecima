add_executable(ProjectDS
        src/main.cpp
        src/decima/archive/archive.cpp
        src/decima/archive/archive_manager.cpp
        src/decima/archive/archive_tree.cpp
        src/decima/archive/archive_file.cpp
        src/utils.cpp
        src/app.cpp
        src/projectds_app.cpp
        src/projectds_app_draw.cpp
        src/decima/serializable/object/object.cpp
        src/decima/serializable/object/object_draw.cpp
        src/decima/serializable/object/object_dummy.cpp
        src/decima/serializable/object/collection.cpp
        src/decima/serializable/object/collection_draw.cpp
        src/decima/serializable/object/prefetch.cpp
        src/decima/serializable/object/prefetch_draw.cpp
        src/decima/serializable/object/translation.cpp
        src/decima/serializable/object/translation_draw.cpp
        src/decima/serializable/object/texture.cpp
        src/decima/serializable/object/texture_draw.cpp
        src/decima/serializable/object/texture_set.cpp
        src/decima/serializable/object/texture_set_draw.cpp
        src/decima/serializable/reference.cpp
        src/decima/serializable/string.cpp
        src/decima/serializable/stream.cpp
        src/decima/serializable/guid.cpp
        src/decima/serializable/handlers.cpp
        src/decima/serializable/object/resource/vertex_array_resource.cpp
        src/decima/serializable/object/resource/vertex_array_resource_draw.cpp
        src/decima/serializable/object/resource/index_array_resource.cpp
        src/decima/serializable/object/resource/index_array_resource_draw.cpp
        src/decima/serializable/object/resource/primitive_resource.cpp
        src/decima/serializable/object/resource/primitive_resource_draw.cpp)

target_link_libraries(ProjectDS PRIVATE hash imgui glfw glad)
target_include_directories(ProjectDS PRIVATE include)

if (MSVC)
    target_compile_definitions(ProjectDS PUBLIC _CRT_SECURE_NO_WARNINGS _ITERATOR_DEBUG_LEVEL=0)
    target_compile_options(ProjectDS PUBLIC /EHs)
endif ()