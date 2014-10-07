cmake_minimum_required (VERSION 2.8)
project (Pictus)
if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")
endif()

find_package(Boost 1.50.0 REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
link_libraries(${Boost_LIBRARIES} )

add_subdirectory (deps)
