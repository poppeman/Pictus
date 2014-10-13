cmake_minimum_required (VERSION 2.8)

FILE(GLOB ALL_HEADER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB ALL_CPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB ALL_C_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

set(PSD ${CMAKE_CURRENT_SOURCE_DIR}/../Pictus/)

include_directories(${DEPS_DIRECTORY} ${CMAKE_SOURCE_DIR}/source)
link_directories(${illa_BINARY_DIR}/illa)

add_executable(pictusTest
    ${ALL_CPP_SRCS}
    ${ALL_C_SRCS}
    ${DEPS_DIRECTORY}
    ${PSD}/image_dimensions.cpp
    ${PSD}/view_pan.cpp
    ${PSD}/w32_rs_tilemanager.cpp
    ${PSD}/w32_renderer.cpp
    ${PSD}/w32_ddsurface.cpp
    ${PSD}/wintypes.cpp
    ${PSD}/zoomstrategy.cpp
    )
target_link_libraries(pictusTest illa UnitTestPP ${Boost_LIBRARIES})

add_custom_command(TARGET pictusTest POST_BUILD COMMAND pictusTest ${CMAKE_SOURCE_DIR}/bin/Test COMMENT "Running pictusTest...")
