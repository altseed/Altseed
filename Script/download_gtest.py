
import aceutils

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):
	aceutils.rm(r"gtest-1.7.0.zip")
	aceutils.rmdir(r"gtest-1.7.0")
	aceutils.rmdir(r"gtest_bin")
	aceutils.rmdir(r"gtest_bin_x64")

	aceutils.wget(r"http://googletest.googlecode.com/files/gtest-1.7.0.zip")
	aceutils.unzip(r"gtest-1.7.0.zip")
	aceutils.mkdir(r"gtest_bin")
	aceutils.mkdir(r"gtest_bin_x64")

	with aceutils.CurrentDir('gtest_bin'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake+r'-D BUILD_SHARED_LIBS:BOOL=OFF ../gtest-1.7.0/')
			aceutils.call(aceutils.cmd_compile + r'gtest.sln /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'gtest.sln /p:configuration=Release')
		elif aceutils.isMac():
			aceutils.call(r'cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX=../Dev "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../gtest-1.7.0/')
			aceutils.call(r'make')
		else:
			aceutils.call(r'cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX=../Dev ../gtest-1.7.0/')
			aceutils.call(r'make')

	with aceutils.CurrentDir('gtest_bin_x64'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake_x64+r'-D BUILD_SHARED_LIBS:BOOL=OFF ../gtest-1.7.0/')
			aceutils.call(aceutils.cmd_compile + r'gtest.sln /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'gtest.sln /p:configuration=Release')

	if aceutils.isWin():
		aceutils.copy(r'gtest_bin/Debug/gtest.lib', r'../Dev/lib/x86/Debug/gtest.lib')
		aceutils.copy(r'gtest_bin/Debug/gtest_main.lib', r'../Dev/lib/x86/Debug/gtest_main.lib')
		aceutils.copy(r'gtest_bin/Release/gtest.lib', r'../Dev/lib/x86/Release/gtest.lib')
		aceutils.copy(r'gtest_bin/Release/gtest_main.lib', r'../Dev/lib/x86/Release/gtest_main.lib')

		aceutils.copy(r'gtest_bin_x64/Debug/gtest.lib', r'../Dev/lib/x64/Debug/gtest.lib')
		aceutils.copy(r'gtest_bin_x64/Debug/gtest_main.lib', r'../Dev/lib/x64/Debug/gtest_main.lib')
		aceutils.copy(r'gtest_bin_x64/Release/gtest.lib', r'../Dev/lib/x64/Release/gtest.lib')
		aceutils.copy(r'gtest_bin_x64/Release/gtest_main.lib', r'../Dev/lib/x64/Release/gtest_main.lib')

	else:
		aceutils.copy(r'gtest_bin/libgtest.a', r'../Dev/lib/libgtest.a')
		aceutils.copy(r'gtest_bin/libgtest_main.a', r'../Dev/lib/libgtest_main.a')

	aceutils.copytree(r'gtest-1.7.0/include/gtest', r'../Dev/include/gtest', True)








