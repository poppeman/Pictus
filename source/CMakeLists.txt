cmake_minimum_required (VERSION 2.8)

if (WIN32)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -std=c++1y -municode")
  SET(Boost_USE_STATIC_LIBS ON)
else()
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
endif()

find_package(Boost 1.50.0 COMPONENTS system thread REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
link_libraries(${Boost_LIBRARIES})

add_subdirectory (deps)
add_subdirectory (orzTest)
add_subdirectory (illaTest)
add_subdirectory (pictustest)
add_subdirectory (Pictus)
