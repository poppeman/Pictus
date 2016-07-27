cmake_minimum_required (VERSION 3.0)

find_package(wxWidgets REQUIRED COMPONENTS base)

SET(ALL_CPP_SRCS matrix.cpp ogl_device.cpp ogl_texture.cpp)

include(${wxWidgets_USE_FILE})
include_directories(${DEPS_DIRECTORY} ${wxWidgets_INCLUDE_DIRS})

add_library(hw3d STATIC ${ALL_CPP_SRCS} ${ALL_HEADER_SRCS})
