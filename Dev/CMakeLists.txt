# Debug Releaseに限定
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE )

# プロジェクト名
project(Altseed)

# CMake バージョンチェック
if (MSVC)
cmake_minimum_required(VERSION 2.8.11)
else()
cmake_minimum_required(VERSION 2.8)
endif()

# MSVC向け設定
if (MSVC)
	option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" OFF)
endif()

# コンパイル設定
if (MSVC)
	# 特に何も
else()
	set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC" ${CMAKE_CXX_FLAGS})
endif()

# Effekseerの設定
if (APPLE)
	option(USE_GLEW_STATIC "Use Glew static library" OFF)
	option(USE_GLEW_DLL "Use Glew DLL" OFF)
	option(USE_OPENGL3 "Use OpenGL 3" ON)
else()
	option(USE_GLEW_STATIC "Use Glew static library" OFF)
	option(USE_GLEW_DLL "Use Glew DLL" ON)
endif()
option(USE_OPENGLES2 "Use OpenGL ES2" OFF)
option(USE_INTERNAL_LOADER "Use Internal Loader" ON)

# ファイルをソースリストに追加
INCLUDE(CMakeFiles.txt)

add_subdirectory(asd_cpp/common/)

if (WIN32)
	list(APPEND core_src ${win_src})
	list(APPEND core_src ${dx11_src})
	list(APPEND core_src ${gl_src})
else()
	list(APPEND core_src ${x11_src})
	list(APPEND core_src ${gl_src})
endif()

# インクルードの設定
include_directories(
	include/
	include/freetype/
	asd_cpp/common/
	asd_cpp/tool/
	asd_cpp/core/
	)

#ライブラリの設定
link_directories(${PROJECT_SOURCE_DIR}/lib/)

# パッケージの設定
set(common_lib)
set(common_inc)

if(MSVC)

else()
	list(APPEND common_lib glfw3 )
	if(NOT APPLE)
		# MacではGLEW不使用
		list(APPEND common_lib GLEW )
	endif()
	list(APPEND common_lib png16 )
	list(APPEND common_lib z )
	list(APPEND common_lib gtest )
	list(APPEND common_lib gtest_main )

	list(APPEND common_lib Effekseer )
	list(APPEND common_lib EffekseerRendererGL )

	# OpenSoundMixerの後に内部で使用しているライブラリを追加する必要あり
	list(APPEND common_lib OpenSoundMixer )
	list(APPEND common_lib ogg )
	list(APPEND common_lib vorbisfile )
	list(APPEND common_lib vorbis )

	list(APPEND common_lib Culling2D )
	list(APPEND common_lib Culling3D )

	list(APPEND common_lib BulletCollision )
	list(APPEND common_lib LinearMath )
	
	list(APPEND common_lib Box2D )

	if(MSVC)
		list(APPEND common_lib gd_static )
	else()
		list(APPEND common_lib gd )
	endif()

	list(APPEND common_lib freetype)

	if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		FIND_PACKAGE(OpenAL REQUIRED)
		list(APPEND common_lib ${OPENAL_LIBRARY})
	else()
		FIND_PACKAGE(PulseAudio REQUIRED)
		list(APPEND common_lib ${PulseAudio_LIBRARY})
	endif()

	list(APPEND common_lib dl )

	find_package(Threads REQUIRED)
	list(APPEND common_lib ${CMAKE_THREAD_LIBS_INIT})

	find_package(OpenGL)
	list(APPEND common_inc ${OPENGL_INCLUDE_DIR})
	list(APPEND common_lib ${OPENGL_LIBRARY})

	find_package(JNI)
	list(APPEND common_inc ${JNI_INCLUDE_DIRS})
	list(APPEND common_lib ${JNI_LIBRARIES})

	if(NOT APPLE)
		find_package(X11 REQUIRED)
		list(APPEND common_inc ${X11_INCLUDE_DIR})
		list(APPEND common_lib ${X11_LIBRARIES})

		if (NOT X11_Xrandr_FOUND)
			message(FATAL_ERROR "X11_Xrandr were not found")
		endif()
		list(APPEND common_inc ${X11_Xrandr_INCLUDE_PATH})
		list(APPEND common_lib ${X11_Xrandr_LIB})

		if (NOT X11_Xinput_FOUND)
			message(FATAL_ERROR "X11_Xinput were not found")
		endif()
		list(APPEND common_inc ${X11_Xinput_INCLUDE_PATH})
		list(APPEND common_lib ${X11_Xinput_LIB})

		if (NOT X11_Xcursor_FOUND)
			message(FATAL_ERROR "X11_Xcursor were not found")
		endif()
		list(APPEND common_inc ${X11_Xcursor_INCLUDE_PATH})
		list(APPEND common_lib ${X11_Xcursor_LIB})


		if (NOT X11_Xinerama_FOUND)
			message(FATAL_ERROR "X11_Xinerama were not found")
		endif()
		list(APPEND common_inc ${X11_Xinerama_INCLUDE_PATH})
		list(APPEND common_lib ${X11_Xinerama_LIB})


		if (NOT X11_xf86vmode_FOUND)
			message(FATAL_ERROR "X11_xf86vmode were not found")
		endif()

		list(APPEND common_inc ${X11_xf86vmode_INCLUDE_PATH})
		list(APPEND common_lib ${X11_Xxf86vm_LIB})
	endif()

	if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		# used in GLFW
		find_library(COCOA_FRAMEWORK Cocoa)
		find_library(COCOA_FRAMEWORK Cocoa)
		find_library(IOKIT_FRAMEWORK IOKit)
		find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
		find_library(CORE_VIDEO_FRAMEWORK CoreVideo)
		list(APPEND common_lib
			${COCOA_FRAMEWORK}
			${OPENGL_gl_LIBRARY}
			${OPENGL_glu_LIBRARY}
			${IOKIT_FRAMEWORK}
			${CORE_FOUNDATION_FRAMEWORK}
			${CORE_VIDEO_FRAMEWORK})
	endif()

endif()

# ディレクトリ設定
# http://stackoverflow.com/questions/31422680/how-to-set-visual-studio-filters-for-nested-sub-directory-using-cmake

foreach(src ${core_src})
	get_filename_component(src_path_dir "${src}" DIRECTORY)
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" src_path "${src_path_dir}")
	string(REPLACE "/" "\\" src_path_r "${src_path}")
	source_group("${src_path_r}" FILES "${src}")
endforeach()

foreach(src ${tool_src})
	get_filename_component(src_path_dir "${src}" DIRECTORY)
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" src_path "${src_path_dir}")
	string(REPLACE "/" "\\" src_path_r "${src_path}")
	source_group("${src_path_r}" FILES "${src}")
endforeach()

foreach(src ${unittest_gtest_src})
	get_filename_component(src_path_dir "${src}" DIRECTORY)
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" src_path "${src_path_dir}")
	string(REPLACE "/" "\\" src_path_r "${src_path}")
	source_group("${src_path_r}" FILES "${src}")
endforeach()

foreach(src ${fontGenerator_src})
	get_filename_component(src_path_dir "${src}" DIRECTORY)
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" src_path "${src_path_dir}")
	string(REPLACE "/" "\\" src_path_r "${src_path}")
	source_group("${src_path_r}" FILES "${src}")
endforeach()



# 共通インクルードの追加
include_directories( AFTER SYSTEM ${common_inc} )

# コンパイル先を追加
#add_library( asdCommon STATIC ${common_src} )

add_library( asdTool STATIC ${tool_src} )

add_library( asdCore STATIC ${core_src} )
add_library( asdCoreDLL SHARED ${core_src} )

target_link_libraries(asdCoreDLL asdTool asd_common ${common_lib}) 

add_library( asdEngine STATIC ${engine_src} ${common_to_engine_src} )

add_executable( asdUnitTestGtest ${unittest_gtest_src} )
target_link_libraries(asdUnitTestGtest asdCore asdTool asd_common ${common_lib}) 

add_executable( asdUnitTestEngineGtest ${unittest_engine_gtest_src} )
target_link_libraries(asdUnitTestEngineGtest asdEngine ${common_lib}) 

add_library( FontGeneratorCore SHARED ${fontGenerator_src} )
target_link_libraries(FontGeneratorCore asdTool asd_common ${common_lib}) 

# 出力先の設定
#set_target_properties(asdCommon PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "lib/" )
#set_target_properties(asdCommon PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib/" )

set_target_properties(asdTool PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "lib/" )
set_target_properties(asdTool PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib/" )

set_target_properties(asdCore PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "lib/" )
set_target_properties(asdCore PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib/" )

set_target_properties(asdCoreDLL PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "bin/" )
set_target_properties(asdCoreDLL PROPERTIES LIBRARY_OUTPUT_DIRECTORY "bin/" )
set_target_properties(asdCoreDLL PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin/" )

set_target_properties(asdEngine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "lib/" )
set_target_properties(asdEngine PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib/" )

set_target_properties(asdUnitTestGtest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin/" )
set_target_properties(asdUnitTestEngineGtest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin/" )

set_target_properties(FontGeneratorCore PROPERTIES LIBRARY_OUTPUT_DIRECTORY "bin/" )

#if (MSVC)
#set_target_properties(asdCommon PROPERTIES OUTPUT_NAME_DEBUG "asd_common" )
#set_target_properties(asdCommon PROPERTIES OUTPUT_NAME_RELEASE "asd_common" )
#else()
#set_target_properties(asdCommon PROPERTIES OUTPUT_NAME "asd_common" )
#endif()

if (MSVC)
set_target_properties(asdTool PROPERTIES OUTPUT_NAME_DEBUG "asd_tool" )
set_target_properties(asdTool PROPERTIES OUTPUT_NAME_RELEASE "asd_tool" )
else()
set_target_properties(asdTool PROPERTIES OUTPUT_NAME "asd_tool" )
endif()

if (MSVC)
set_target_properties(asdCore PROPERTIES OUTPUT_NAME_DEBUG "asd_core" )
set_target_properties(asdCore PROPERTIES OUTPUT_NAME_RELEASE "asd_core" )
else()
set_target_properties(asdCore PROPERTIES OUTPUT_NAME "asd_core" )
endif()

if (MSVC)
set_target_properties(asdCoreDLL PROPERTIES OUTPUT_NAME_DEBUG "Altseed_core" )
set_target_properties(asdCoreDLL PROPERTIES OUTPUT_NAME_RELEASE "Altseed_core" )
else()
set_target_properties(asdCoreDLL PROPERTIES OUTPUT_NAME "Altseed_core" )
endif()

if (MSVC)
set_target_properties(asdEngine PROPERTIES OUTPUT_NAME_DEBUG "Altseed" )
set_target_properties(asdEngine PROPERTIES OUTPUT_NAME_RELEASE "Altseed" )
else()
set_target_properties(asdEngine PROPERTIES OUTPUT_NAME "Altseed" )
endif()

set_target_properties(asdUnitTestGtest PROPERTIES OUTPUT_NAME_DEBUG "UnitTestGtest_cpp" )
set_target_properties(asdUnitTestGtest PROPERTIES OUTPUT_NAME_RELEASE "UnitTestGtest_cpp" )

set_target_properties(asdUnitTestEngineGtest PROPERTIES OUTPUT_NAME_DEBUG "UnitTestEngineGtest_cpp" )
set_target_properties(asdUnitTestEngineGtest PROPERTIES OUTPUT_NAME_RELEASE "UnitTestEngineGtest_cpp" )

# Linux x86_64向けの追加のリンクオプション
if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	set_target_properties(asdTool PROPERTIES COMPILE_FLAGS "-fPIC")
endif()


# フラグ処理
if (MSVC)
	if (NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
		foreach (flag CMAKE_C_FLAGS
			CMAKE_C_FLAGS_DEBUG
			CMAKE_C_FLAGS_RELEASE
			CMAKE_CXX_FLAGS
			CMAKE_CXX_FLAGS_DEBUG
			CMAKE_CXX_FLAGS_RELEASE)

			if (${flag} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
			endif()
			if (${flag} MATCHES "/MDd")
				string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
			endif()
		endforeach()
	endif()
endif()

# コンパイラ取得
if (MSVC)
	add_definitions(-D_$(PlatformToolset))
endif()

# Effekseerの設定
if (USE_INTERNAL_LOADER)
	add_definitions(-D__EFFEKSEER_RENDERER_INTERNAL_LOADER__)
endif()

if (USE_GLEW_STATIC)
	add_definitions(-D__EFFEKSEER_RENDERER_GL_GLEW_S__)
endif()

if (USE_GLEW_DLL)
	add_definitions(-D__EFFEKSEER_RENDERER_GL_GLEW__)
endif()

if (USE_OPENGLES2)
	add_definitions(-D__EFFEKSEER_RENDERER_GLES2__)
endif()

if (USE_OPENGLES3)
	add_definitions(-D__EFFEKSEER_RENDERER_GLES3__)
endif()

if (USE_OPENGL3)
	add_definitions(-D__EFFEKSEER_RENDERER_GL3__)
endif()

# インストール先を決定
#install(
#	TARGETS asdCommon
#	ARCHIVE DESTINATION lib
#	LIBRARY DESTINATION lib)

install(
	TARGETS asdTool
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib)

install(
	TARGETS asdCore
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib )

install(
	TARGETS asdCoreDLL
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION bin
	LIBRARY DESTINATION bin )

install(
	TARGETS asdEngine
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib )

install(
	TARGETS asdUnitTestGtest
	RUNTIME DESTINATION bin )

install(
	TARGETS asdUnitTestEngineGtest
	RUNTIME DESTINATION bin )

install(
	TARGETS FontGeneratorCore
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION bin
	LIBRARY DESTINATION bin )

