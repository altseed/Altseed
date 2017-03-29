
# Debug or Release
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE )

# Minimum
cmake_minimum_required(VERSION 2.8)

# Set project
project(unitTest_Engine_cpp_gtest)

# Add codes
FILE(GLOB_RECURSE cpps *.cpp)
FILE(GLOB_RECURSE hs *.h)
list(APPEND srcs ${cpps} ${hs})

# Assgin directory
# http://stackoverflow.com/questions/31422680/how-to-set-visual-studio-filters-for-nested-sub-directory-using-cmake

foreach(src ${srcs})
	get_filename_component(src_path_dir "${src}" DIRECTORY)
	string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" src_path "${src_path_dir}")
	string(REPLACE "/" "\\" src_path_r "${src_path}")
	source_group("${src_path_r}" FILES "${src}")
endforeach()

# Set MSVC Flags
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


if(NOT MSVC AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-Wl,--no-as-needed -lX11 -ldl")
endif()

include_directories(../include/)

link_directories(${PROJECT_SOURCE_DIR}/../lib/)

add_executable( unitTest_Engine_cpp_gtest ${srcs} )

set_target_properties(unitTest_Engine_cpp_gtest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../bin/" )

if (MSVC)
set_target_properties(unitTest_Engine_cpp_gtest PROPERTIES OUTPUT_NAME_DEBUG "unitTest_Engine_cpp_gtest" )
set_target_properties(unitTest_cpp_gtest PROPERTIES OUTPUT_NAME_RELEASE "unitTest_Engine_cpp_gtest" )
else()
set_target_properties(unitTest_Engine_cpp_gtest PROPERTIES OUTPUT_NAME "unitTest_Engine_cpp_gtest" )
endif()

target_link_libraries(unitTest_Engine_cpp_gtest asdEngine gtest gtest_main) 
