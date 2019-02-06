# Limit to Debug Release
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE )

# Project name
project(Altseed)

# Option
option(BUILD_TOOL "Build tool" OFF)

# Check CMake Version
if (MSVC)
cmake_minimum_required(VERSION 3.1)
else()
cmake_minimum_required(VERSION 3.1)
endif()

# C++11
set (CMAKE_CXX_STANDARD 11)

# For MSVC
if (MSVC)
	option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" OFF)
endif()

# For Apple
if(APPLE)
	set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12)
endif()

# Add flags
if (MSVC)
	# Nothing
else()
	set(CMAKE_CXX_FLAGS " -fPIC -fpermissive ${CMAKE_CXX_FLAGS}")
	set(CMAKE_C_FLAGS "-fPIC ${CMAKE_C_FLAGS}")

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
add_subdirectory(asd_cpp/tool/)

# ======== Core ========

# 3rd
FILE(GLOB_RECURSE third_srcs
	asd_cpp/core/3rdParty/poly2tri/*.cpp
	asd_cpp/core/3rdParty/poly2tri/*.h

	asd_cpp/core/3rdParty/imgui/imgui.cpp
	asd_cpp/core/3rdParty/imgui/imgui_draw.cpp
	asd_cpp/core/3rdParty/imgui/imgui_impl_glfw.cpp
	asd_cpp/core/3rdParty/imgui/imgui_impl_opengl3.cpp
	asd_cpp/core/3rdParty/nfd/nfd_common.cpp
	asd_cpp/core/3rdParty/Boxer/boxer.h
)

if (WIN32)
FILE(GLOB_RECURSE third_win_srcs
	asd_cpp/core/3rdParty/DirectXToolKit/*.cpp
	asd_cpp/core/3rdParty/DirectXToolKit/*.h
	asd_cpp/core/3rdParty/imgui/imgui_impl_dx11.cpp
	asd_cpp/core/3rdParty/nfd/nfd_win.cpp
	asd_cpp/core/3rdParty/Boxer/boxer_win.cpp
)

list(APPEND third_srcs ${third_win_srcs})

endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
FILE(GLOB_RECURSE third_mac_srcs
	asd_cpp/core/3rdParty/nfd/nfd_cocoa.m
	asd_cpp/core/3rdParty/Boxer/boxer_osx.mm
)

list(APPEND third_srcs ${third_mac_srcs})

endif()



FILE(GLOB_RECURSE core_srcs
	asd_cpp/core/Collision/*.cpp
	asd_cpp/core/Collision/*.h
	asd_cpp/core/Core/*.cpp
	asd_cpp/core/Core/*.h
	asd_cpp/core/Graphics/*.cpp
	asd_cpp/core/Graphics/*.h
	asd_cpp/core/Input/*.cpp
	asd_cpp/core/Input/*.h
	asd_cpp/core/IO/*.cpp
	asd_cpp/core/IO/*.h
	asd_cpp/core/Log/*.cpp
	asd_cpp/core/Log/*.h
	asd_cpp/core/ObjectSystem/*.cpp
	asd_cpp/core/ObjectSystem/*.h
	asd_cpp/core/PCH/*.cpp
	asd_cpp/core/PCH/*.h
	asd_cpp/core/Profiler/*.cpp
	asd_cpp/core/Profiler/*.h
	asd_cpp/core/Shape/*.cpp
	asd_cpp/core/Shape/*.h
	asd_cpp/core/Sound/*.cpp
	asd_cpp/core/Sound/*.h
	asd_cpp/core/Utils/*.cpp
	asd_cpp/core/Utils/*.h
	asd_cpp/core/Window/*.cpp
	asd_cpp/core/Window/*.h
	asd_cpp/core/Tool/*.cpp
	asd_cpp/core/Tool/*.h
)

FILE(GLOB core_root_srcs
	asd_cpp/core/*.cxx
	asd_cpp/core/*.cpp
	asd_cpp/core/*.h
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
FILE(GLOB_RECURSE core_mac_srcs
	asd_cpp/core/Graphics/*.mm
)

list(APPEND core_srcs ${core_mac_srcs})

endif()

list(APPEND core_srcs ${core_root_srcs} ${third_srcs})

foreach(FILE ${core_srcs}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    # Group into "Source Files" and "Header Files"
    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.c")
       set(GROUP "Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.cxx")
       set(GROUP "Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "Files${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

# ======== ==== ========


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

	list(APPEND common_lib AltseedPlatform)
	list(APPEND common_lib AltseedRHI)

	list(APPEND common_lib OpenSoundMixer )

	list(APPEND common_lib Culling2D )
	list(APPEND common_lib Culling2D_AABB )

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
	#list(APPEND common_lib ${JNI_LIBRARIES})

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
			${CORE_VIDEO_FRAMEWORK}
			"-framework AVFoundation"
			"-framework CoreMedia")
	endif()

endif()

# 共通インクルードの追加
include_directories( AFTER SYSTEM ${common_inc} )

# コンパイル先を追加
#add_library( asdCommon STATIC ${common_src} )

add_library( asdCore STATIC ${core_srcs} )
add_library( asdCoreDLL SHARED ${core_srcs} )

target_link_libraries(asdCoreDLL asd_tool asd_common ${common_lib}) 

add_library( asdEngine STATIC ${engine_src} ${common_to_engine_src} )

add_library( FontGeneratorCore SHARED ${fontGenerator_src} )
target_link_libraries(FontGeneratorCore asd_tool asd_common ${common_lib}) 

# 出力先の設定
set_target_properties(asdCore PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "lib/" )
set_target_properties(asdCore PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib/" )

set_target_properties(asdCoreDLL PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "bin/" )
set_target_properties(asdCoreDLL PROPERTIES LIBRARY_OUTPUT_DIRECTORY "bin/" )
set_target_properties(asdCoreDLL PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin/" )

set_target_properties(asdEngine PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "lib/" )
set_target_properties(asdEngine PROPERTIES LIBRARY_OUTPUT_DIRECTORY "lib/" )

set_target_properties(FontGeneratorCore PROPERTIES LIBRARY_OUTPUT_DIRECTORY "bin/" )

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
	TARGETS FontGeneratorCore
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION bin
	LIBRARY DESTINATION bin )


# Tests
add_subdirectory(unitTest_cpp_gtest/)
add_subdirectory(unitTest_Engine_cpp_gtest/)

# Tool
if (BUILD_TOOL)
	add_subdirectory(ImagePackageGenerator/)
	set_target_properties(PSDParser PROPERTIES LIBRARY_OUTPUT_DIRECTORY "bin/" )

endif()

if (BUILD_TOOL_3D)
	add_subdirectory(FBX2MDL/)
endif()

