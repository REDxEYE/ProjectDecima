add_executable(ArchiveTest
        src/archive_test.cpp
        src/archive.cpp
        src/file_types/prefetch.cpp
        src/archive_array.cpp
        src/utils.cpp
        src/file_types/prefetch.cpp
        src/file_types/core.cpp
        src/archive_file.cpp)

target_include_directories(ArchiveTest PUBLIC include ${HASHLIB_INC} ${OOZLIB_INC})


target_link_libraries(ArchiveTest PUBLIC
        HashLib oozLib
        -static-libgcc -static-libstdc++ -static
        -Wl,-Bstatic -lstdc++ -lpthread
        -Wl,-Bdynamic
        -fuse-ld=lld)
