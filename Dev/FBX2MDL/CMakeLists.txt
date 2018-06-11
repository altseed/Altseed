set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE )

project(FBX2MDL)

cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

message("Please set environment FBX_INCLUDE and FBX_LIB, which contains fbx sdk.")

endif()

set(srcs
	FBX2MDL.FBXExporter.cpp
	FBX2MDL.FBXImporter.cpp
	FBX2MDL.FBXOptimizer.cpp
	main.cpp)

# Flags
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

add_subdirectory(../asd_cpp/common/)
add_subdirectory(../asd_cpp/tool/)

include_directories(
	../asd_cpp/common/
	../asd_cpp/tool/
	$ENV{FBX_INCLUDE})

link_directories(
	$ENV{FBX_LIB})

add_executable(FBX2MDL ${srcs})

if (WIN32)

target_link_libraries(FBX2MDL
	asd_common
	asd_tool
	fbxsdk-mt)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

target_link_libraries(FBX2MDL
	asd_common
	asd_tool
	fbxsdk)

endif()



