add_executable(ProjectDS
        src/main.cpp
        src/archive.cpp
        src/file_types/prefetch.cpp
        src/archive_array.cpp
        src/utils.cpp
        src/file_types/prefetch.cpp
        src/archive_tree.cpp
        src/file_types/core.cpp
        src/archive_file.cpp)

target_include_directories(ProjectDS PUBLIC include ${HASHLIB_INC} ${OOZLIB_INC} ${LGLE_INCLUDE} ${LGLE_LIB_INCLUDE})


target_link_libraries(ProjectDS PUBLIC
        HashLib oozLib LGLE
        -static-libgcc -static-libstdc++ -static
        -Wl,-Bstatic -lstdc++ -lpthread
        -Wl,-Bdynamic
        -fuse-ld=lld)

target_precompile_headers(ProjectDS PUBLIC <tchar.h> libs/LGLE/lib/imgui/imgui.h include/imgui_memory_editor.h include/portable-file-dialogs.h <unordered_map> <vector>)
