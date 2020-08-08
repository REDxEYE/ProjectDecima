add_executable(ArchiveTest
        src/archive_test.cpp
        src/decima/archive/archive.cpp
        src/decima/archive/archive_file.cpp
        src/decima/archive/archive_array.cpp
        src/decima/file_types/prefetch.cpp
        src/decima/file_types/prefetch.cpp
        src/decima/file_types/core.cpp
        src/decima/file_types/core_draw.cpp
        src/utils.cpp
        #        src/decima/file_types/core_draw.cpp
        )

target_include_directories(ArchiveTest PUBLIC include ${HASHLIB_INC} ${OOZLIB_INC})


target_link_libraries(ArchiveTest PUBLIC
        HashLib oozLib imgui
        -static-libgcc -static-libstdc++ -static
        -Wl,-Bstatic -lstdc++ -lpthread
        -Wl,-Bdynamic
        -fuse-ld=lld)
