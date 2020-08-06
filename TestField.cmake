add_executable(TestField
        src/test_field.cpp
        src/decima/file_types/core.cpp
        src/utils.cpp
        )


target_include_directories(TestField PUBLIC include ${HASHLIB_INC} ${OOZLIB_INC})

target_link_libraries(TestField PUBLIC
        HashLib oozLib
        -static-libgcc -static-libstdc++ -static
        -Wl,-Bstatic -lstdc++ -lpthread
        -Wl,-Bdynamic
        -fuse-ld=lld)
