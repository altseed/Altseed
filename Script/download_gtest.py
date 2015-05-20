
import aceutils

aceutils.cdToScript()
aceutils.cd(r"../")

aceutils.rm(r"gtest-1.7.0.zip")

aceutils.rmdir(r"gtest-1.7.0")

aceutils.wget(r"http://googletest.googlecode.com/files/gtest-1.7.0.zip")

aceutils.unzip(r"gtest-1.7.0.zip")

aceutils.rmdir(r"gtest_bin")
aceutils.rmdir(r"gtest_bin_x64")

aceutils.mkdir(r"gtest_bin")
aceutils.mkdir(r"gtest_bin_x64")


aceutils.cd(r"gtest_bin")

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12" -D BUILD_SHARED_LIBS:BOOL=OFF ../gtest-1.7.0/')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" gtest.sln /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" gtest.sln /p:configuration=Release')
else:
	aceutils.call(r'cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX=../Dev ../gtest-1.7.0/')
	aceutils.call(r'make')

aceutils.cd(r"../")

aceutils.cd(r"gtest_bin_x64")

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12 Win64" -D BUILD_SHARED_LIBS:BOOL=OFF ../gtest-1.7.0/')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" gtest.sln /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" gtest.sln /p:configuration=Release')

aceutils.cd(r"../")

if aceutils.isWin():
	aceutils.copy(r'gtest_bin/Debug/gtest.lib', r'Dev/lib/x86/Debug/gtest.lib')
	aceutils.copy(r'gtest_bin/Debug/gtest_main.lib', r'Dev/lib/x86/Debug/gtest_main.lib')
	aceutils.copy(r'gtest_bin/Release/gtest.lib', r'Dev/lib/x86/Release/gtest.lib')
	aceutils.copy(r'gtest_bin/Release/gtest_main.lib', r'Dev/lib/x86/Release/gtest_main.lib')

	aceutils.copy(r'gtest_bin_x64/Debug/gtest.lib', r'Dev/lib/x64/Debug/gtest.lib')
	aceutils.copy(r'gtest_bin_x64/Debug/gtest_main.lib', r'Dev/lib/x64/Debug/gtest_main.lib')
	aceutils.copy(r'gtest_bin_x64/Release/gtest.lib', r'Dev/lib/x64/Release/gtest.lib')
	aceutils.copy(r'gtest_bin_x64/Release/gtest_main.lib', r'Dev/lib/x64/Release/gtest_main.lib')

else:
	aceutils.copy(r'gtest_bin/libgtest.a', r'Dev/lib/libgtest.a')
	aceutils.copy(r'gtest_bin/libgtest_main.a', r'Dev/lib/libgtest_main.a')

aceutils.copytree(r'gtest-1.7.0/include/gtest', r'Dev/include/gtest', True)








