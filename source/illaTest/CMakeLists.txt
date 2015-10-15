cmake_minimum_required (VERSION 2.8)

FILE(GLOB ALL_HEADER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB ALL_CPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB ALL_C_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

include_directories(${DEPS_DIRECTORY})
link_directories(${illa_BINARY_DIR}/illa)

add_executable(illaTest ${ALL_CPP_SRCS} ${ALL_C_SRCS} ${DEPS_DIRECTORY})
target_link_libraries(illaTest illa UnitTestPP ${Boost_LIBRARIES})

add_custom_command(TARGET illaTest POST_BUILD COMMAND illaTest ${CMAKE_SOURCE_DIR}/bin/Test COMMENT "Running illaTest...")
