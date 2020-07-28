cmake_minimum_required(VERSION 3.15)
project(ProjectDS)

add_subdirectory(libs/hash)
add_subdirectory(libs/ooz)
add_subdirectory(libs/LGLE)

set(CMAKE_CXX_STANDARD 17)

add_executable(ProjectDS
        main.cpp
        src/archive/decima_archive.cpp
        src/file_types/prefetch.cpp
        src/archive_array.cpp
        src/utils.cpp
        src/file_types/prefetch.cpp
        src/memory_stream.cpp
        ${LGLE_SRC}
        src/file_tree.cpp)


target_include_directories(ProjectDS PUBLIC include ${HASHLIB_INC} ${OOZLIB_INC} ${LGLE_INCLUDE} ${LGLE_LIB_INCLUDE})

target_link_libraries(ProjectDS PUBLIC
        HashLib oozLib ${LGLE_LINK_OPTS} -fuse-ld=lld)


