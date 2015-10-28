cmake_minimum_required (VERSION 2.8)

if (WIN32)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -std=c++1y -municode")
  SET(Boost_USE_STATIC_LIBS ON)
else()
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
endif()


if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

find_package(Boost 1.55.0 COMPONENTS system thread locale filesystem REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
link_libraries(${Boost_LIBRARIES})

add_subdirectory (deps)
add_subdirectory (orzTest)
add_subdirectory (illaTest)
add_subdirectory (LoadTest)

if(WIN32)
	add_subdirectory (PictThumbs)
	add_subdirectory (pictustest)
	add_subdirectory (Pictus)
endif()
