cmake_minimum_required(VERSION 3.10)
project(detex)

set(DETEX_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/detex)
set(PNG_BUILD_ZLIB ON)

add_library(detex STATIC
        ${DETEX_SOURCE_DIR}/bits.c
        ${DETEX_SOURCE_DIR}/bptc-tables.c
        ${DETEX_SOURCE_DIR}/clamp.c
        ${DETEX_SOURCE_DIR}/convert.c
        ${DETEX_SOURCE_DIR}/dds.c
        ${DETEX_SOURCE_DIR}/decompress-bc.c
        ${DETEX_SOURCE_DIR}/decompress-bptc.c
        ${DETEX_SOURCE_DIR}/decompress-bptc-float.c
        ${DETEX_SOURCE_DIR}/decompress-eac.c
        ${DETEX_SOURCE_DIR}/decompress-etc.c
        ${DETEX_SOURCE_DIR}/decompress-rgtc.c
        ${DETEX_SOURCE_DIR}/division-tables.c
        ${DETEX_SOURCE_DIR}/file-info.c
        ${DETEX_SOURCE_DIR}/half-float.c
        ${DETEX_SOURCE_DIR}/hdr.c
        ${DETEX_SOURCE_DIR}/ktx.c
        ${DETEX_SOURCE_DIR}/misc.c
        ${DETEX_SOURCE_DIR}/texture.c
        ${DETEX_SOURCE_DIR}/png.c
        )

target_include_directories(detex PUBLIC ${DETEX_SOURCE_DIR} ./)
target_link_libraries(detex PRIVATE zlibstatic png_static)