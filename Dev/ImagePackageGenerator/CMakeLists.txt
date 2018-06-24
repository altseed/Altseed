# Debug Releaseに限定
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE )

# プロジェクト名
project(ImagePackageGenerator)

# CMake バージョンチェック
cmake_minimum_required(VERSION 2.8)

if (MSVC)

else()
	set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC" ${CMAKE_CXX_FLAGS})
endif()

# ソースコード設定
set(src
	PSDParser/PSDParser.cpp
	PSDParser/dll.cxx
	PSDParser/psd/psd.c
	PSDParser/psd/list/list.c
	PSDParser/psd/list/list_iterator.c
	PSDParser/psd/list/list_node.c)

# コンパイル先を追加
add_library( PSDParser SHARED ${src} )

# フラグ処理
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

# 出力先の設定
if (MSVC)
set_target_properties(PSDParser PROPERTIES OUTPUT_NAME_DEBUG "PSDParser" )
set_target_properties(PSDParser PROPERTIES OUTPUT_NAME_RELEASE "PSDParser" )
else()
set_target_properties(PSDParser PROPERTIES OUTPUT_NAME "PSDParser" )
endif()
