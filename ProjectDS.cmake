add_executable(ProjectDS
        src/main.cpp
        src/decima/archive/archive.cpp
        src/decima/archive/archive_array.cpp
        src/decima/archive/archive_tree.cpp
        src/decima/archive/archive_file.cpp
        src/imgui_backend_impl.cpp
        src/utils.cpp
        src/app.cpp
        src/projectds_app.cpp
        src/projectds_app_draw.cpp
        src/projectds_app_utils.cpp
        src/decima/file_types/core/core.cpp
        src/decima/file_types/core/core_draw.cpp
        src/decima/file_types/core/dummy.cpp
        src/decima/file_types/core/prefetch.cpp
        src/decima/file_types/core/translation.cpp
        src/decima/file_types/core/translation_draw.cpp
        src/decima/file_types/core/texture.cpp
        src/decima/file_types/core/texture_draw.cpp
        src/decima/file_types/pod/string.cpp
        src/decima/file_types/pod/stream.cpp
        src/decima/file_types/pod/guid.cpp)

target_include_directories(ProjectDS PUBLIC include ${HASHLIB_INC} ${OOZLIB_INC})

target_include_directories(ProjectDS PUBLIC
        libs/glad/include
        libs/glfw/include
        )

target_link_libraries(ProjectDS PUBLIC
        HashLib oozLib glfw glad imgui detex
        -static-libgcc -static-libstdc++ -static
        -Wl,-Bstatic -lstdc++ -lpthread
        -Wl,-Bdynamic
        -fuse-ld=lld)
