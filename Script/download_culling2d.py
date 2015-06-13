
import aceutils

aceutils.cdToScript()
aceutils.cd(r'../')
aceutils.rm(r'master.zip')
aceutils.rmdir(r'Culling2D-master')

aceutils.wget(r'https://github.com/altseed/Culling2D/archive/master.zip')
aceutils.unzip(r'master.zip')

aceutils.editCmakeForACE(r'Culling2D-master/CMakeLists.txt','cp932')

aceutils.rmdir(r"culling2d_bin")
aceutils.rmdir(r"culling2d_bin_x64")

aceutils.mkdir(r"culling2d_bin")
aceutils.mkdir(r"culling2d_bin_x64")

aceutils.cd(r"culling2d_bin")

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../Culling2D-master/')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Culling2D.sln /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Culling2D.sln /p:configuration=Release')
elif aceutils.isMac():
	aceutils.call(r'cmake -G "Unix Makefiles" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../Culling2D-master/')
	aceutils.call(r'make')
else:
	aceutils.call(r'cmake -G "Unix Makefiles" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../Culling2D-master/')
	aceutils.call(r'make')

aceutils.cd(r"../")

aceutils.cd(r"culling2d_bin_x64")

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12 Win64" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../Culling2D-master/')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Culling2D.sln /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Culling2D.sln /p:configuration=Release')

aceutils.cd(r"../")

if aceutils.isWin():
	aceutils.mkdir(r'Dev/lib/x86/')
	aceutils.mkdir(r'Dev/lib/x86/Debug')
	aceutils.mkdir(r'Dev/lib/x86/Release')

	aceutils.mkdir(r'Dev/lib/x64/')
	aceutils.mkdir(r'Dev/lib/x64/Debug')
	aceutils.mkdir(r'Dev/lib/x64/Release')

	aceutils.copy(r'Culling2D-master/Culling2D/Culling2D.h', r'Dev/include/')

	aceutils.copy(r'culling2d_bin/Debug/Culling2D_debug.lib', r'Dev/lib/x86/Debug/Culling2D.lib')
	aceutils.copy(r'culling2d_bin/Release/Culling2D.lib', r'Dev/lib/x86/Release/')
	aceutils.copy(r'culling2d_bin_x64/Debug/Culling2D_debug.lib', r'Dev/lib/x64/Debug/Culling2D.lib')
	aceutils.copy(r'culling2d_bin_x64/Release/Culling2D.lib', r'Dev/lib/x64/Release/')

else:
	aceutils.copy(r'Culling2D-master/Culling2D/Culling2D.h', r'Dev/include/')
	aceutils.copy(r'culling2d_bin/bin/libCulling2D.a', r'Dev/lib/')


