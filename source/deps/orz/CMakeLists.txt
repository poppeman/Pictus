CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

FIND_PACKAGE(wxWidgets COMPONENTS core base gl net REQUIRED)

SET(
	ORZ_SRC
	bitconvert.cpp bitconvert.h
	ByteOrder.cpp ByteOrder.h 
	exception.cpp exception.h
	fileops.cpp fileops.h
	file_cache.cpp file_cache.h
	file_reader.cpp file_reader.h
	file_writer.cpp file_writer.h
	folder.cpp folder.h
	folder_monitor.cpp folder_monitor.h
	folder_monitor_imp.cpp folder_monitor_imp.h
	folder_types.cpp folder_types.h
	intl.cpp intl.h
	intl_language.h
	logger.cpp logger.h
	matrix.cpp matrix.h
	orz_math.cpp orz_math.h
	stopwatch.cpp stopwatch.h
	stream.cpp stream.h
	streamconv.cpp streamconv.h
	stream_file.cpp stream_file.h
	stream_mem.cpp stream_mem.h
	sysinfo.h
	threadpool.cpp threadpool.h
	types.cpp types.h
)

SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/intl_language.h PROPERTIES GENERATED 1)
SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/intl.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/intl_language.h)

IF(WIN32)
	LIST(
		APPEND ORZ_SRC
		Win32/sysinfo_win.cpp
		Win32/fileops_win.cpp
	)
	SET(
		PlatformLibraries
		shlwapi
	)
ELSE()
	FIND_PACKAGE(GLIB COMPONENTS gio gobject REQUIRED)
	LIST(
		APPEND ORZ_SRC
		Linux/sysinfo_linux.cpp
		Linux/linux_folder_monitor.cpp
		Linux/fileops_linux.cpp
	)
	SET(
		PlatformIncludes
		${GLIB_INCLUDE_DIRS}
	)
	SET(
		PlatformLibraries
		${GLIB_LIBRARIES}
		${GLIB_GIO_LIBRARIES}
		${GLIB_GOBJECT_LIBRARIES}
	)
ENDIF()

ADD_CUSTOM_COMMAND(
	OUTPUT
		${CMAKE_CURRENT_SOURCE_DIR}/intl_language.h
		${CMAKE_CURRENT_SOURCE_DIR}/../../Pictus/lang_val.h
		${CMAKE_CURRENT_SOURCE_DIR}/../../Pictus/lang_val.cpp
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../Pictus
	COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/../../Pictus/lang_convert.py
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../../Pictus/lang_convert.py
)

INCLUDE(${wxWidgets_USE_FILE})
INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS} ${PlatformIncludes})
ADD_LIBRARY(orz STATIC ${ORZ_SRC})
TARGET_LINK_LIBRARIES(orz ${wxWidgets_LIBRARIES} ${Boost_LIBRARIES} ${PlatformLibraries})
