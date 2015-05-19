
import aceutils
import shutil

aceutils.cdToScript()
aceutils.cd(r'../')
aceutils.rm(r'Box2D_V2.3.0.7z')
aceutils.rmdir(r'Box2D_V2.3.0')

aceutils.wget(r'https://box2d.googlecode.com/files/Box2D_v2.3.0.7z')

if aceutils.isWin():
	aceutils.call(r'7za x -y Box2D_v2.3.0.7z')
else:
	aceutils.call(r'7za x -y Box2D_v2.3.0.7z')


aceutils.editCmakeForACE(r'Box2D_v2.3.0/Box2D/CMakeLists.txt','cp932')
aceutils.editCmakeForACE(r'Box2D_v2.3.0/Box2D/Box2D/CMakeLists.txt','cp932')

aceutils.rmdir(r"box2d_bin")
aceutils.rmdir(r"box2d_bin_x64")

aceutils.mkdir(r"box2d_bin")
aceutils.mkdir(r"box2d_bin_x64")

aceutils.cd(r"box2d_bin")

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D BOX2D_BUILD_EXAMPLES:BOOL=OFF ../Box2D_v2.3.0/Box2D/')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Box2D.sln /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Box2D.sln /p:configuration=Release')
else:
	aceutils.call(r'cmake -G "Unix Makefiles" ../Box2D_v2.3.0/Box2D/')
	aceutils.call(r'make')

aceutils.cd(r"../")


aceutils.cd(r"box2d_bin_x64")

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12 Win64" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D BOX2D_BUILD_EXAMPLES:BOOL=OFF ../Box2D_v2.3.0/Box2D/')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Box2D.sln /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Box2D.sln /p:configuration=Release')

aceutils.cd(r"../")

if aceutils.isWin():
	aceutils.mkdir(r'Dev/lib/Debug')
	aceutils.mkdir(r'Dev/lib/Release')
	aceutils.copytree(r'Box2D_v2.3.0/Box2D/Box2D', r'Dev/include/Box2D/',ignoreList=shutil.ignore_patterns(r'*.txt',r'*.cpp',r'*.cmake'))
	
	aceutils.copy(r'box2d_bin/Box2D/Debug/Box2D.lib', r'Dev/lib/x86/Debug/')
	aceutils.copy(r'box2d_bin/Box2D/Release/Box2D.lib', r'Dev/lib/x86/Release/')
	aceutils.copy(r'box2d_bin_x64/Box2D/Debug/Box2D.lib', r'Dev/lib/x64/Debug/')
	aceutils.copy(r'box2d_bin_x64/Box2D/Release/Box2D.lib', r'Dev/lib/x64/Release/')

else:
	aceutils.copytree(r'Box2D_v2.3.0/Box2D/Box2D', r'Dev/include/Box2D/')
	aceutils.copy(r'box2d_bin/Box2D/libBox2D.a', r'Dev/lib/')