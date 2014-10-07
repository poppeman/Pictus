cmake_minimum_required (VERSION 2.8)
project (orz)

FILE(GLOB ALL_HEADER_SRCS ${Pictus_SOURCE_DIR}/deps/orz/*.h)
FILE(GLOB ALL_CPP_SRCS ${Pictus_SOURCE_DIR}/deps/orz/*.cpp)

add_library(orz STATIC ${ALL_CPP_SRCS} ${ALL_HEADER_SRCS})
