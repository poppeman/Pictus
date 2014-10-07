cmake_minimum_required (VERSION 2.8)

FILE(GLOB ALL_HEADER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB ALL_CPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

include_directories(${DEPS_DIRECTORY})

add_library(png STATIC ${ALL_CPP_SRCS} ${ALL_HEADER_SRCS})
target_link_libraries(png zlib)