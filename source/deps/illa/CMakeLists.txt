cmake_minimum_required (VERSION 2.8)

find_package(PNG REQUIRED)
find_package(TIFF REQUIRED)
find_package(ZLIB REQUIRED)

FILE(GLOB ALL_HEADER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB ALL_CPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

include_directories(${DEPS_DIRECTORY} ${PNG_INCLUDE_DIRS} ${TIFF_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})

add_library(illa STATIC ${ALL_CPP_SRCS} ${ALL_HEADER_SRCS})
target_link_libraries(illa orz tiff jpeg png webp webpdemux Metadata ${Boost_LIBRARIES} ${PNG_LIBRARIES} ${TIFF_LIBRARIES} ${ZLIB_LIBRARIES})
