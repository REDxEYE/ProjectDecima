add_executable(ProjectDS
        src/main.cpp
        src/decima/archive/archive.cpp
        src/decima/file_types/prefetch.cpp
        src/decima/archive/archive_array.cpp
        src/utils.cpp
        src/decima/file_types/prefetch.cpp
        src/decima/archive/archive_tree.cpp
        src/decima/file_types/translation.cpp
        src/decima/file_types/core.cpp
        src/decima/archive/archive_file.cpp
        src/imgui_backend_impl.cpp
        src/app.cpp src/projectds_app.cpp
        src/projectds_app_draw.cpp
        src/decima/file_types/dummy.cpp
        src/decima/file_types/core_draw.cpp
        src/projectds_app_utils.cpp
        src/decima/file_types/translation_draw.cpp src/decima/file_types/texture.cpp src/decima/file_types/texture_draw.cpp)

target_include_directories(ProjectDS PUBLIC include ${HASHLIB_INC} ${OOZLIB_INC})

target_include_directories(ProjectDS PUBLIC
        libs/glad/include
        libs/glfw/include
        )

target_link_libraries(ProjectDS PUBLIC
        HashLib oozLib glfw glad imgui
        -static-libgcc -static-libstdc++ -static
        -Wl,-Bstatic -lstdc++ -lpthread
        -Wl,-Bdynamic
        -fuse-ld=lld)
