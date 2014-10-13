cmake_minimum_required (VERSION 2.8)

FILE(GLOB ALL_HEADER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB ALL_CPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB ALL_C_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

include_directories(${DEPS_DIRECTORY})
link_directories(${orz_BINARY_DIR}/orz)

add_executable(orzTest ${ALL_CPP_SRCS} ${ALL_C_SRCS} ${DEPS_DIRECTORY})
target_link_libraries(orzTest orz UnitTestPP ${Boost_LIBRARIES})

add_custom_command(TARGET orzTest POST_BUILD COMMAND orzTest ${CMAKE_SOURCE_DIR}/bin/Test COMMENT "Running orzTest...")
