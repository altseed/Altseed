import aceutils

aceutils.cd(r'../')
aceutils.rm(r'master.zip')
aceutils.rmdir(r'glfw-master')
aceutils.wget(r'https://github.com/ac-engine/GLFW/archive/master.zip')
aceutils.unzip(r'master.zip')

aceutils.editCmakeForACE(r'glfw-master/CMakeLists.txt')

if aceutils.isWin():
	aceutils.cd(r"glfw-master")
else:
	aceutils.mkdir(r"glfw_bin")
	aceutils.cd(r"glfw_bin")

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../glfw-master/')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" ALL_BUILD.vcxproj /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" ALL_BUILD.vcxproj /p:configuration=Release')
else:
	aceutils.call(r'cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX=../Dev ../glfw-master/')
	aceutils.call(r'make install')

aceutils.cd(r"../")

aceutils.mkdir(r"Dev/include/GLFW/")

if aceutils.isWin():
	aceutils.copy(r'glfw-master/include/GLFW/glfw3.h', r'Dev/include/GLFW/')
	aceutils.copy(r'glfw-master/include/GLFW/glfw3native.h', r'Dev/include/GLFW')

	aceutils.copy(r'glfw-master/src/Debug/glfw3.lib', r'Dev/lib/Debug/')
	aceutils.copy(r'glfw-master/src/Release/glfw3.lib', r'Dev/lib/Release/')

else:
	aceutils.copy(r'glfw-master/include/GLFW/glfw3.h', r'Dev/include/GLFW/')
	aceutils.copy(r'glfw-master/include/GLFW/glfw3native.h', r'Dev/include/GLFW')