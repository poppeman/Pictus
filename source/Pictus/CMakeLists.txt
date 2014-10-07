cmake_minimum_required (VERSION 2.8)
project (Pictus)

FILE(GLOB ALL_HEADER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB PICTUS_CPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

include_directories(${DEPS_DIRECTORY})
link_directories(${illa_BINARY_DIR}/illa ${orz_BINARY_DIR}/orz)

add_executable(Pictus ${PICTUS_CPP_SRCS} ${DEPS_DIRECTORY})
target_link_libraries(Pictus illa orz d3dwrap comctl32 ${Boost_LIBRARIES})
