import aceutils

aceutils.cdToScript()
aceutils.cd(r'../')
aceutils.rm(r'master.zip')
aceutils.rmdir(r'glfw-master')
aceutils.wget(r'https://github.com/altseed/GLFW/archive/master.zip')
aceutils.unzip(r'master.zip')

aceutils.editCmakeForACE(r'glfw-master/CMakeLists.txt')

aceutils.rmdir(r"glfw_bin")
aceutils.rmdir(r"glfw_bin_x64")

aceutils.mkdir(r"glfw_bin")
aceutils.mkdir(r"glfw_bin_x64")

aceutils.cd(r"glfw_bin")

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../glfw-master/')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" ALL_BUILD.vcxproj /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" ALL_BUILD.vcxproj /p:configuration=Release')
elif aceutils.isMac():
	aceutils.call(r'cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX:PATH=../Dev "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../glfw-master/')
	aceutils.call(r'make install')
else:
	aceutils.call(r'cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX=../Dev ../glfw-master/')
	aceutils.call(r'make install')

aceutils.cd(r"../")

aceutils.cd(r"glfw_bin_x64")

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12 Win64" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../glfw-master/')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" ALL_BUILD.vcxproj /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" ALL_BUILD.vcxproj /p:configuration=Release')

aceutils.cd(r"../")

aceutils.mkdir(r"Dev/include/GLFW/")

if aceutils.isWin():

	aceutils.mkdir(r'Dev/lib/x86/')
	aceutils.mkdir(r'Dev/lib/x86/Debug')
	aceutils.mkdir(r'Dev/lib/x86/Release')

	aceutils.mkdir(r'Dev/lib/x64/')
	aceutils.mkdir(r'Dev/lib/x64/Debug')
	aceutils.mkdir(r'Dev/lib/x64/Release')

	aceutils.copy(r'glfw-master/include/GLFW/glfw3.h', r'Dev/include/GLFW/')
	aceutils.copy(r'glfw-master/include/GLFW/glfw3native.h', r'Dev/include/GLFW')

	aceutils.copy(r'glfw_bin/src/Debug/glfw3.lib', r'Dev/lib/x86/Debug/')
	aceutils.copy(r'glfw_bin/src/Release/glfw3.lib', r'Dev/lib/x86/Release/')

	aceutils.copy(r'glfw_bin_x64/src/Debug/glfw3.lib', r'Dev/lib/x64/Debug/')
	aceutils.copy(r'glfw_bin_x64/src/Release/glfw3.lib', r'Dev/lib/x64/Release/')

else:
	aceutils.copy(r'glfw-master/include/GLFW/glfw3.h', r'Dev/include/GLFW/')
	aceutils.copy(r'glfw-master/include/GLFW/glfw3native.h', r'Dev/include/GLFW')