
import aceutils
import os.path

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):
	aceutils.rmdir(r"gtest_bin")
	aceutils.rmdir(r"gtest_bin_x64")

	aceutils.mkdir(r"gtest_bin")
	aceutils.mkdir(r"gtest_bin_x64")

	with aceutils.CurrentDir('gtest_bin'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake+r'-D BUILD_SHARED_LIBS:BOOL=OFF ../googletest/ "-DCMAKE_CXX_FLAGS=/source-charset:utf-8 /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING"')
			aceutils.call('cmake --build . --config Debug')
			aceutils.call('cmake --build . --config Release')

		elif aceutils.isMac():
			aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX=../Dev -DBUILD_GTEST=ON -DBUILD_GMOCK=OFF "-DCMAKE_OSX_ARCHITECTURES=x86_64' + (';i386' if aceutils.Isi386() else '') + r'" ../googletest/')
			aceutils.call(r'make')
		else:
			aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX=../Dev ../googletest/')
			aceutils.call(r'make')

	with aceutils.CurrentDir('gtest_bin_x64'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake_x64+r'-D BUILD_SHARED_LIBS:BOOL=OFF ../googletest/ "-DCMAKE_CXX_FLAGS=/source-charset:utf-8 /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING"')
			aceutils.call('cmake --build . --config Debug')
			aceutils.call('cmake --build . --config Release')

	if aceutils.isWin():
		src_path = 'gtest_bin/googlemock/gtest/'
		if os.path.exists('gtest_bin/gtest.sln'):
			src_path = 'gtest_bin/'

		aceutils.copy(src_path+r'Debug/gtest.lib', r'../Dev/lib/x86/Debug/gtest.lib')
		aceutils.copy(src_path+r'Debug/gtest_main.lib', r'../Dev/lib/x86/Debug/gtest_main.lib')
		aceutils.copy(src_path+r'Release/gtest.lib', r'../Dev/lib/x86/Release/gtest.lib')
		aceutils.copy(src_path+r'Release/gtest_main.lib', r'../Dev/lib/x86/Release/gtest_main.lib')

		src_path = 'gtest_bin_x64/googlemock/gtest/'
		if os.path.exists('gtest_bin_x64/gtest.sln'):
			src_path = 'gtest_bin_x64/'

		aceutils.copy(src_path+r'Debug/gtest.lib', r'../Dev/lib/x64/Debug/gtest.lib')
		aceutils.copy(src_path+r'Debug/gtest_main.lib', r'../Dev/lib/x64/Debug/gtest_main.lib')
		aceutils.copy(src_path+r'Release/gtest.lib', r'../Dev/lib/x64/Release/gtest.lib')
		aceutils.copy(src_path+r'Release/gtest_main.lib', r'../Dev/lib/x64/Release/gtest_main.lib')

	else:
		aceutils.copy(r'gtest_bin/googletest/libgtest.a', r'../Dev/lib/libgtest.a')
		aceutils.copy(r'gtest_bin/googletest/libgtest_main.a', r'../Dev/lib/libgtest_main.a')

	aceutils.copytree(r'googletest/googletest/include/gtest', r'../Dev/include/gtest', True)








