cmake_minimum_required (VERSION 2.8)
project (Pictus)

find_package(wxWidgets COMPONENTS core base gl net REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

FILE(GLOB ALL_HEADER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
set(PICTUS_CPP_SRCS
		adjust.cpp adjust.h
		app_types.cpp
		builder_viewport.cpp
		clipboard.cpp clipboard.h
		cnt_color_hls.cpp cnt_color_hls.h
		cnt_color_rgb.cpp cnt_color_rgb.h
		cnt_colorpick.cpp cnt_colorpick.h
		cnt_colorpreview.cpp cnt_colorpreview.h
		config_path.cpp config_path.h
		ctrl_keypress.cpp ctrl_keypress.h
		dlg_about.cpp
		dlg_advanced.cpp dlg_advanced.h
		dlg_cache.cpp dlg_cache.h
		dlg_color.cpp dlg_color.h
		dlg_controls.cpp dlg_controls.h
		dlg_interface.cpp dlg_interface.h
		dlg_keyboard.cpp dlg_keyboard.h
		dlg_language.cpp dlg_language.h
		dlg_view.cpp
		filemanager.cpp filemanager.h
		filterstring.cpp
		getevent.cpp
		hw3d_device.cpp
		hw3d_staging_texture.cpp
		hw3d_texture.cpp
		hw3d_opengl_common.cpp
		hw3d_opengl_context.cpp
		hw3d_opengl_device.cpp
		hw3d_opengl_texture.cpp
		hw3d_opengl_staging_texture.cpp
		hw3d_opengl_staging_texture_pbo.cpp
		hw3d_opengl_staging_texture_simple.cpp
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
		ipc_client.cpp ipc_client.h
		ipc_openfile_connection.cpp ipc_openfile_connection.h
		ipc_server.cpp ipc_server.h
		lang_val.cpp
		main.cpp
		settings.cpp
		settings_layout.cpp
		settings_page.cpp
		reg_color_translator.cpp
		reg_filter_mode_translator.cpp
		reg_keyboard_binding_translator.cpp
		reg_language_translator.cpp
		reg_mouse_action_translator.cpp
		reg_resize_behavior_translator.cpp
		reg_resize_position_method_translator.cpp
		reg_zoom_mode_translator.cpp
		registry.cpp
		rename.cpp rename.h
		render_geometry.cpp
		resize_position_method.cpp
		timer.cpp
		version.cpp
		view_pan.cpp
		viewer.cpp
		viewer_contextmenu.cpp viewer_contextmenu.h
		viewer_droptarget.cpp viewer_droptarget.h
		viewer_keyboard.cpp
		viewport.cpp
		w32_redrawstrategy.cpp
		w32_renderer.cpp
		w32_rendertarget.cpp
		w32_rs_tiled.cpp
		w32_rs_tilemanager.cpp
		wintypes.cpp
		zoomstrategy.cpp)

SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/lang_val.cpp PROPERTIES GENERATED 1)

FILE(GLOB PICTUS_RC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.rc)
message(${wxWidgets_INCLUDE_DIRS})
message(${wxWidgets_FOUND})

include(${wxWidgets_USE_FILE})
include_directories(${DEPS_DIRECTORY} ${wxWidgets_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
link_directories(${illa_BINARY_DIR}/illa ${orz_BINARY_DIR}/orz)

if(MINGW)
	set(CMAKE_RC_COMPILER_INIT windres)
	ENABLE_LANGUAGE(RC)
	SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif(MINGW)

add_executable(Pictus ${PICTUS_CPP_SRCS} ${PICTUS_RC_SRCS} ${DEPS_DIRECTORY})
target_link_libraries(Pictus illa orz ${Boost_LIBRARIES} ${wxWidgets_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
add_dependencies(Pictus orzTest illaTest pictusTest)
