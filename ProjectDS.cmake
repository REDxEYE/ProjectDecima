add_executable(ProjectDS
        src/main.cpp
        src/decima/archive/archive.cpp
        src/decima/archive/archive_array.cpp
        src/decima/archive/archive_tree.cpp
        src/decima/core_file.cpp
        src/utils.cpp
        src/app.cpp
        src/projectds_app.cpp
        src/projectds_app_draw.cpp
        src/projectds_app_utils.cpp
        src/decima/file_types/core/entry.cpp
        src/decima/file_types/core/core_draw.cpp
        src/decima/file_types/core/dummy_entry.cpp
        src/decima/file_types/core/prefetch.cpp
        src/decima/file_types/core/prefetch_draw.cpp
        src/decima/file_types/core/translation.cpp
        src/decima/file_types/core/translation_draw.cpp
        src/decima/file_types/core/texture.cpp
        src/decima/file_types/core/texture_draw.cpp
        src/decima/file_types/pod/string.cpp
        src/decima/file_types/pod/stream.cpp
        src/decima/file_types/pod/guid.cpp
        src/decima/file_types/core/texture_set.cpp
        src/decima/file_types/core/texture_set_draw.cpp
        src/decima/file_types/pod/reference.cpp
        src/decima/file_types/core/collection.cpp)

target_link_libraries(ProjectDS PRIVATE oodle hash imgui glfw glad)
target_include_directories(ProjectDS PRIVATE include)

if (MSVC)
    target_compile_definitions(ProjectDS PUBLIC _CRT_SECURE_NO_WARNINGS)
    target_compile_options(ProjectDS PUBLIC /EHs)
endif ()