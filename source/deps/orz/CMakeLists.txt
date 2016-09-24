cmake_minimum_required (VERSION 2.8)

FILE(GLOB ALL_HEADER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/intl_language.h)
FILE(GLOB ALL_CPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/intl_language.h PROPERTIES GENERATED 1)
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/intl.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/intl_language.h)

if(WIN32)
  list(APPEND ALL_CPP_SRCS Win32/sysinfo_win.cpp)
else()
  list(APPEND ALL_CPP_SRCS Linux/sysinfo_linux.cpp Linux/linux_folder_monitor.cpp)
endif()

ADD_CUSTOM_COMMAND(
	OUTPUT
		${CMAKE_CURRENT_SOURCE_DIR}/intl_language.h
		${CMAKE_CURRENT_SOURCE_DIR}/../../Pictus/lang_val.h
		${CMAKE_CURRENT_SOURCE_DIR}/../../Pictus/lang_val.cpp
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../Pictus
	COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/../../Pictus/lang_convert.py
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../../Pictus/lang_convert.py
)

add_library(orz STATIC ${ALL_CPP_SRCS} ${ALL_HEADER_SRCS} Linux/linux_folder_monitor.h)
set(libraries ${Boost_LIBRARIES})
if(WIN32)
  list(APPEND libraries shlwapi)
endif()
target_link_libraries(orz ${libraries})
