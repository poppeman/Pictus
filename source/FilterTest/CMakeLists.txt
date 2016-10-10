cmake_minimum_required (VERSION 2.8)
project (FilterTest)

include_directories(${DEPS_DIRECTORY})
add_executable(FilterTest main.cpp)
target_link_libraries(FilterTest illa orz ${Boost_LIBRARIES})
