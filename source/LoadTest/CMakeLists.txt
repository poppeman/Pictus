cmake_minimum_required (VERSION 2.8)
project (LoadTest)

include_directories(${DEPS_DIRECTORY})
link_directories(${illa_BINARY_DIR}/illa ${orz_BINARY_DIR}/orz)

add_executable(LoadTest main.cpp)
target_link_libraries(LoadTest illa orz ${Boost_LIBRARIES})
