add_executable(NoLGLEProjectDS
        src/no_lgle_main.cpp
        src/archive.cpp
        src/file_types/prefetch.cpp
        src/archive_array.cpp
        src/utils.cpp
        src/file_types/prefetch.cpp
        src/archive_tree.cpp
        src/file_types/core.cpp
        src/archive_file.cpp src/imgui_backend_impl.cpp)

target_include_directories(NoLGLEProjectDS PUBLIC include ${HASHLIB_INC} ${OOZLIB_INC})

target_include_directories(NoLGLEProjectDS PUBLIC
        libs/LGLE/lib/glad/include
        libs/LGLE/lib/glfw/include
        )

target_link_libraries(NoLGLEProjectDS PUBLIC
        HashLib oozLib glfw glad imgui
        -static-libgcc -static-libstdc++ -static
        -Wl,-Bstatic -lstdc++ -lpthread
        -Wl,-Bdynamic
        -fuse-ld=lld)

target_precompile_headers(NoLGLEProjectDS PUBLIC <tchar.h> libs/LGLE/lib/imgui/imgui.h include/imgui_memory_editor.h include/portable-file-dialogs.h <unordered_map> <vector>)