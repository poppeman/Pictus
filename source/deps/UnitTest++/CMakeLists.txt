cmake_minimum_required (VERSION 2.8)

FILE(GLOB ALL_HEADER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB ALL_CPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

if(WIN32)
	list(APPEND ALL_CPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/Win32/TimeHelpers.cpp)
else()
	list(APPEND ALL_CPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/Posix/SignalTranslator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Posix/TimeHelpers.cpp)
endif()

include_directories(${DEPS_DIRECTORY})
add_library(UnitTestPP STATIC ${ALL_CPP_SRCS} ${ALL_HEADER_SRCS})
