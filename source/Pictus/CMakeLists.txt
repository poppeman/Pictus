cmake_minimum_required (VERSION 2.8)
project (Pictus)

find_package(wxWidgets REQUIRED COMPONENTS core base)

FILE(GLOB ALL_HEADER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB PICTUS_CPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
FILE(GLOB PICTUS_RC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.rc)
message(${wxWidgets_INCLUDE_DIRS})
message(${wxWidgets_FOUND})

include(${wxWidgets_USE_FILE})
include_directories(${DEPS_DIRECTORY} ${wxWidgets_INCLUDE_DIRS})
link_directories(${illa_BINARY_DIR}/illa ${orz_BINARY_DIR}/orz)

if(MINGW)
  set(CMAKE_RC_COMPILER_INIT windres)
  ENABLE_LANGUAGE(RC)
  SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif(MINGW)

add_executable(Pictus ${PICTUS_CPP_SRCS} ${PICTUS_RC_SRCS} ${DEPS_DIRECTORY})
target_link_libraries(Pictus illa orz d3dwrap comctl32 ${Boost_LIBRARIES})
add_dependencies(Pictus orzTest illaTest pictusTest)
