cmake_minimum_required (VERSION 2.8)
project (Pictus)

find_package(wxWidgets COMPONENTS core base gl REQUIRED)
find_package(OpenGL REQUIRED)

FILE(GLOB ALL_HEADER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
set(PICTUS_CPP_SRCS
		app_types.cpp
		builder_viewport.cpp
		filterstring.cpp
		getevent.cpp
		hw3d_device.cpp
		hw3d_texture.cpp
		hw3d_opengl_context.cpp
		hw3d_opengl_device.cpp
		hw3d_opengl_texture.cpp
		factory_renderer.cpp
		ic_backgroundprocessor.cpp
		ic_imageloader.cpp
		ic_queue.cpp
		ic_workerthread.cpp
		ic_worklist.cpp
		ic_workpackage.cpp
		imagecache.cpp
		imagecache_fileentry.cpp
		imagecache_workrequest.cpp
		lang_val.cpp
		main.cpp
		reg_color_translator.cpp
		reg_filter_mode_translator.cpp
		reg_keyboard_binding_translator.cpp
		reg_language_translator.cpp
		reg_mouse_action_translator.cpp
		reg_resize_behavior_translator.cpp
		reg_resize_position_method_translator.cpp
		reg_zoom_mode_translator.cpp
		registry.cpp
		render_geometry.cpp
		resize_position_method.cpp
		timer.cpp
		view_pan.cpp
		viewer.cpp
		viewer_keyboard.cpp
		viewport.cpp
		w32_assure_folder.cpp
		w32_redrawstrategy.cpp
		w32_renderer.cpp
		w32_rendertarget.cpp
		w32_rs_tiled.cpp
		w32_rs_tilemanager.cpp
		wintypes.cpp
		zoomstrategy.cpp)

FILE(GLOB PICTUS_RC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.rc)
message(${wxWidgets_INCLUDE_DIRS})
message(${wxWidgets_FOUND})

include(${wxWidgets_USE_FILE})
include_directories(${DEPS_DIRECTORY} ${wxWidgets_INCLUDE_DIRS})
link_directories(${illa_BINARY_DIR}/illa ${orz_BINARY_DIR}/orz)

if(MINGW)
	set(CMAKE_RC_COMPILER_INIT windres)
	ENABLE_LANGUAGE(RC)
	SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif(MINGW)

add_executable(Pictus ${PICTUS_CPP_SRCS} ${PICTUS_RC_SRCS} ${DEPS_DIRECTORY})
target_link_libraries(Pictus illa orz ${Boost_LIBRARIES} ${wxWidgets_LIBRARIES} ${OPENGL_LIBRARIES})
add_dependencies(Pictus orzTest illaTest pictusTest)
