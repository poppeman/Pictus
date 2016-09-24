cmake_minimum_required (VERSION 2.8)

if (WIN32)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -std=c++1y -municode")
	SET(Boost_USE_STATIC_LIBS ON)
else()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -Wno-switch -Werror -Wextra -Wno-unused-parameter")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-switch")
endif()

find_package(Boost 1.55.0 COMPONENTS system thread locale filesystem REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
link_libraries(${Boost_LIBRARIES})

ADD_CUSTOM_COMMAND(
	OUTPUT
		${CMAKE_CURRENT_BINARY_DIR}/deps/orz/intl_language.h
		${CMAKE_CURRENT_BINARY_DIR}/Pictus/lang_val.h
		${CMAKE_CURRENT_BINARY_DIR}/Pictus/lang_val.cpp
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Pictus
	COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/Pictus/lang_convert.py
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Pictus/lang_convert.py
)

ADD_CUSTOM_TARGET(GenerateLang DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Pictus/lang_val.cpp)

add_subdirectory (deps)
add_subdirectory (orzTest)
add_subdirectory (illaTest)
add_subdirectory (LoadTest)
add_subdirectory (pictustest)
add_subdirectory (Pictus)

if(WIN32)
	add_subdirectory (PictThumbs)
endif()
