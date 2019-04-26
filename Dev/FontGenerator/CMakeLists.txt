# Limit in Debug and Release
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE )

# Project name
project(ImagePackageGenerator)

# Check CMake version
cmake_minimum_required(VERSION 2.8)

if (MSVC)

else()
	set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC ${CMAKE_CXX_FLAGS}")
endif()

# Add codes
FILE(GLOB cpps 
	FontGeneratorCore/*.cpp
	FontGeneratorCore/Rendering/*.cpp)
FILE(GLOB hs 
	FontGeneratorCore/*.h
	FontGeneratorCore/Rendering/*.h)

list(APPEND srcs ${cpps} ${hs})

# Add library
add_library( FontGeneratorCore SHARED ${srcs} )

# Add dependencies

include_directories(
	../include/
)

link_directories(
    ${PROJECT_SOURCE_DIR}/../lib/
)

if(MSVC)
else()
	target_link_libraries(FontGeneratorCore asd_common asd_tool AltseedRHI freetype)
endif()

# Set flag
if(MSVC)

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

# Set target
if (MSVC)
set_target_properties(FontGeneratorCore PROPERTIES OUTPUT_NAME_DEBUG "FontGeneratorCore" )
set_target_properties(FontGeneratorCore PROPERTIES OUTPUT_NAME_RELEASE "FontGeneratorCore" )
else()
set_target_properties(FontGeneratorCore PROPERTIES OUTPUT_NAME "FontGeneratorCore" )
endif()

set_target_properties(FontGeneratorCore PROPERTIES LIBRARY_OUTPUT_DIRECTORY "bin/" )

install(
	TARGETS FontGeneratorCore
	RUNTIME DESTINATION ../bin
	ARCHIVE DESTINATION ../bin
	LIBRARY DESTINATION ../bin )
