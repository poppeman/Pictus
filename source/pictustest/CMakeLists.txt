cmake_minimum_required (VERSION 2.8)

FILE(GLOB ALL_HEADER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB ALL_CPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB ALL_C_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
FILE(GLOB PICTUS_CPP_SRCS ${Pictus_SOURCE_DIR}/*.cpp)

message(ERROR ${Pictus_SOURCE_DIR})

include_directories(${DEPS_DIRECTORY} ${CMAKE_SOURCE_DIR}/source)
link_directories(${illa_BINARY_DIR}/illa)

add_executable(pictusTest ${ALL_CPP_SRCS} ${ALL_C_SRCS} ${PICTUS_CPP_SRCS} ${DEPS_DIRECTORY})
target_link_libraries(pictusTest illa UnitTestPP ${Boost_LIBRARIES})

add_custom_command(TARGET pictusTest POST_BUILD COMMAND pictusTest ${CMAKE_SOURCE_DIR}/bin/Test COMMENT "Running pictusTest...")
