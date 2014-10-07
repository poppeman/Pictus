cmake_minimum_required (VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -std=c++1y -municode")
SET(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.50.0 COMPONENTS system thread REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
link_libraries(${Boost_LIBRARIES})

add_subdirectory (deps)
add_subdirectory (Pictus)
