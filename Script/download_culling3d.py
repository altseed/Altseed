
import aceutils

aceutils.cdToScript()
aceutils.cd(r'../')
aceutils.rm(r'master.zip')
aceutils.rmdir(r'Culling3D-master')

aceutils.wget(r'https://github.com/ac-engine/Culling3D/archive/master.zip')
aceutils.unzip(r'master.zip')

aceutils.editCmakeForACE(r'Culling3D-master/CMakeLists.txt','cp932')

aceutils.mkdir(r"culling3d_bin")
aceutils.cd(r"culling3d_bin")

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../Culling3D-master/')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Culling3D.sln /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Culling3D.sln /p:configuration=Release')
else:
	aceutils.call(r'cmake -G "Unix Makefiles" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../Culling3D-master/')
	aceutils.call(r'make')

aceutils.cd(r"../")

if aceutils.isWin():
	aceutils.mkdir(r'Dev/lib/Debug')
	aceutils.mkdir(r'Dev/lib/Release')
	aceutils.copy(r'Culling3D-master/Culling3D/Culling3D.h', r'Dev/include/')
	aceutils.copy(r'culling3d_bin/Debug/Culling3D.lib', r'Dev/lib/Debug/')
	aceutils.copy(r'culling3d_bin/Release/Culling3D.lib', r'Dev/lib/Release/')
else:
	aceutils.copy(r'Culling3D-master/Culling3D/Culling3D.h', r'Dev/include/')
	aceutils.copy(r'culling3d_bin/bin/libCulling3D.a', r'Dev/lib/')


