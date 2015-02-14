
import aceutils
import shutil

aceutils.cdToScript()
aceutils.cd(r'../')
aceutils.rm(r'Box2D_V2.3.0.7z')
aceutils.rmdir(r'Box2D_V2.3.0')

aceutils.wget(r'https://box2d.googlecode.com/files/Box2D_v2.3.0.7z')

if aceutils.isWin():
	aceutils.call('7za x -y Box2D_v2.3.0.7z')
else:
	aceutils.call('7za x -y Box2D_v2.3.0.7z')


aceutils.editCmakeForACE(r'Box2D_v2.3.0/Box2D/CMakeLists.txt','cp932')

aceutils.mkdir(r"box2d_bin")
aceutils.cd(r"box2d_bin")

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../Box2D_v2.3.0/Box2D/')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Box2D.sln /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Box2D.sln /p:configuration=Release')
else:
	aceutils.call(r'cmake -G "Unix Makefiles" ../Box2D_v2.3.0/Box2D/')
	aceutils.call(r'make')

aceutils.cd(r"../")

if aceutils.isWin():
	aceutils.mkdir(r'Dev/lib/Debug')
	aceutils.mkdir(r'Dev/lib/Release')
	aceutils.copytree(r'Box2D_v2.3.0/Box2D/Box2D', r'Dev/include/Box2D/',ignoreList=shutil.ignore_patterns('*.txt','*.cpp','*.cmake'))
	
	aceutils.copy(r'box2d_bin/Box2D/Debug/Box2D.lib', r'Dev/lib/Debug/')
	aceutils.copy(r'box2d_bin/Box2D/Release/Box2D.lib', r'Dev/lib/Release/')
else:
	aceutils.copytree(r'Box2D_v2.3.0/Box2D/Box2D', r'Dev/include/Box2D/')
	aceutils.copy(r'box2d_bin/Box2D/libBox2D.a', r'Dev/lib/')