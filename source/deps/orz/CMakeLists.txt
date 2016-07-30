cmake_minimum_required (VERSION 2.8)

FILE(GLOB ALL_HEADER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB ALL_CPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

if(WIN32)
  list(APPEND ALL_CPP_SRCS Win32/sysinfo_win.cpp)
else()
  list(APPEND ALL_CPP_SRCS Linux/sysinfo_linux.cpp Linux/linux_folder_monitor.cpp)
endif()

add_library(orz STATIC ${ALL_CPP_SRCS} ${ALL_HEADER_SRCS} Linux/linux_folder_monitor.h)
set(libraries ${Boost_LIBRARIES})
if(WIN32)
  list(APPEND libraries shlwapi)
endif()
target_link_libraries(orz ${libraries})
