# Debug Release�Ɍ���
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE )

# �v���W�F�N�g��
project(ImagePackageGenerator)

# CMake �o�[�W�����`�F�b�N
cmake_minimum_required(VERSION 2.8)

if (MSVC)

else()
	set(CMAKE_C_FLAGS "-std=c11 -fPIC ${CMAKE_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC ${CMAKE_CXX_FLAGS}")
endif()

# �\�[�X�R�[�h�ݒ�
set(src
	PSDParser/PSDParser.cpp
	PSDParser/PSDParser.Document.cpp
	PSDParser/dll.cxx
	PSDParser/psd/psd.c
	PSDParser/psd/list/list.c
	PSDParser/psd/list/list_iterator.c
	PSDParser/psd/list/list_node.c)

# �R���p�C�����ǉ�
add_library( PSDParser SHARED ${src} )

# �t���O����
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

# �o�͐�̐ݒ�
if (MSVC)
set_target_properties(PSDParser PROPERTIES OUTPUT_NAME_DEBUG "PSDParser" )
set_target_properties(PSDParser PROPERTIES OUTPUT_NAME_RELEASE "PSDParser" )
else()
set_target_properties(PSDParser PROPERTIES OUTPUT_NAME "PSDParser" )
endif()