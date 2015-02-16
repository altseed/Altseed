# Debug Releaseに限定
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE )

# プロジェクト名
project(ace)

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
else()
	option(USE_GLEW_STATIC "Use Glew static library" OFF)
	option(USE_GLEW_DLL "Use Glew DLL" ON)
endif()
option(USE_OPENGLES2 "Use OpenGL ES2" OFF)

# ファイルをソースリストに追加
INCLUDE(CMakeFiles.txt)

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
	ace_cpp/common/
	ace_cpp/tool/
	ace_cpp/core/
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
	
	list(APPEND common_lib Box2D )

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

	find_package(X11 REQUIRED)
	list(APPEND common_inc ${X11_INCLUDE_DIR})
	list(APPEND common_lib ${X11_LIBRARIES})

	find_package(OpenGL)
	list(APPEND common_inc ${OPENGL_INCLUDE_DIR})
	list(APPEND common_lib ${OPENGL_LIBRARY})

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

	if (NOT X11_xf86vmode_FOUND)
		message(FATAL_ERROR "X11_xf86vmode were not found")
	endif()

	list(APPEND common_inc ${X11_xf86vmode_INCLUDE_PATH})
	list(APPEND common_lib ${X11_Xxf86vm_LIB})

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

# コンパイル先を追加
add_library( aceCommon STATIC ${common_src} )

add_library( aceTool STATIC ${tool_src} )

add_library( aceCore STATIC ${core_src} )
add_library( aceCoreDLL SHARED ${core_src} )

target_link_libraries(aceCoreDLL aceTool aceCommon ${common_lib}) 

add_library( aceEngine STATIC ${engine_src} ${common_src} )

add_executable( aceUnitTestGtest ${unittest_gtest_src} )
target_link_libraries(aceUnitTestGtest aceCore aceTool aceCommon ${common_lib}) 

add_executable( aceUnitTestEngineGtest ${unittest_engine_gtest_src} )
target_link_libraries(aceUnitTestEngineGtest aceEngine ${common_lib}) 



# 出力先の設定
set_target_properties(aceCommon PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "lib/" )
set_target_properties(aceCommon PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib/" )

set_target_properties(aceTool PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "lib/" )
set_target_properties(aceTool PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib/" )

set_target_properties(aceCore PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "lib/" )
set_target_properties(aceCore PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib/" )

set_target_properties(aceCoreDLL PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "bin/" )
set_target_properties(aceCoreDLL PROPERTIES LIBRARY_OUTPUT_DIRECTORY "bin/" )
set_target_properties(aceCoreDLL PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin/" )

set_target_properties(aceEngine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "lib/" )
set_target_properties(aceEngine PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib/" )

set_target_properties(aceUnitTestGtest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin/" )
set_target_properties(aceUnitTestEngineGtest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin/" )

if (MSVC)
set_target_properties(aceCommon PROPERTIES OUTPUT_NAME_DEBUG "ace_common" )
set_target_properties(aceCommon PROPERTIES OUTPUT_NAME_RELEASE "ace_common" )
else()
set_target_properties(aceCommon PROPERTIES OUTPUT_NAME "ace_common" )
endif()

if (MSVC)
set_target_properties(aceCore PROPERTIES OUTPUT_NAME_DEBUG "ace_core" )
set_target_properties(aceCore PROPERTIES OUTPUT_NAME_RELEASE "ace_core" )
else()
set_target_properties(aceCore PROPERTIES OUTPUT_NAME "ace_core" )
endif()

if (MSVC)
set_target_properties(aceCoreDLL PROPERTIES OUTPUT_NAME_DEBUG "ace_core" )
set_target_properties(aceCoreDLL PROPERTIES OUTPUT_NAME_RELEASE "ace_core" )
else()
set_target_properties(aceCoreDLL PROPERTIES OUTPUT_NAME "ace_core" )
endif()

if (MSVC)
set_target_properties(aceEngine PROPERTIES OUTPUT_NAME_DEBUG "ace_engine" )
set_target_properties(aceEngine PROPERTIES OUTPUT_NAME_RELEASE "ace_engine" )
else()
set_target_properties(aceEngine PROPERTIES OUTPUT_NAME "ace_engine" )
endif()

set_target_properties(aceUnitTestGtest PROPERTIES OUTPUT_NAME_DEBUG "UnitTestGtest_cpp" )
set_target_properties(aceUnitTestGtest PROPERTIES OUTPUT_NAME_RELEASE "UnitTestGtest_cpp" )

set_target_properties(aceUnitTestEngineGtest PROPERTIES OUTPUT_NAME_DEBUG "UnitTestEngineGtest_cpp" )
set_target_properties(aceUnitTestEngineGtest PROPERTIES OUTPUT_NAME_RELEASE "UnitTestEngineGtest_cpp" )


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

# インストール先を決定
install(
	TARGETS aceCommon
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib)

install(
	TARGETS aceTool
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib)

install(
	TARGETS aceCore
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib )

install(
	TARGETS aceCoreDLL
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION bin
	LIBRARY DESTINATION bin )

install(
	TARGETS aceEngine
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib )

install(
	TARGETS aceUnitTestGtest
	RUNTIME DESTINATION bin )

install(
	TARGETS aceUnitTestEngineGtest
	RUNTIME DESTINATION bin )

