add_executable(QuickHash
        src/quick_hasher.cpp
        src/utils.cpp
        )


target_include_directories(QuickHash PUBLIC include ${HASHLIB_INC} ${OOZLIB_INC})

target_link_libraries(QuickHash PUBLIC
        HashLib oozLib
        -static-libgcc -static-libstdc++ -static
        -Wl,-Bstatic -lstdc++ -lpthread
        -Wl,-Bdynamic
        -fuse-ld=lld)
