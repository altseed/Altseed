
import aceutils
import shutil

aceutils.cdToScript()
aceutils.cd(r'../')
aceutils.rm(r'master.zip')
aceutils.rmdir(r'Box2D-master')

aceutils.wget(r'https://github.com/erincatto/Box2D/archive/master.zip')
aceutils.unzip(r'master.zip');


aceutils.editCmakeForACE(r'Box2D-master/Box2D/CMakeLists.txt','cp932')
aceutils.editCmakeForACE(r'Box2D-master/Box2D/Box2D/CMakeLists.txt','cp932')

aceutils.rmdir(r"box2d_bin")
aceutils.rmdir(r"box2d_bin_x64")

aceutils.mkdir(r"box2d_bin")
aceutils.mkdir(r"box2d_bin_x64")

aceutils.cd(r"box2d_bin")

if aceutils.isWin():
	aceutils.call(aceutils.cmd_cmake+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D BOX2D_BUILD_EXAMPLES:BOOL=OFF ../Box2D-master/Box2D/')
	aceutils.call(aceutils.cmd_compile + r'Box2D.sln /p:configuration=Debug')
	aceutils.call(aceutils.cmd_compile + r'Box2D.sln /p:configuration=Release')
elif aceutils.isMac():
	aceutils.call(r'cmake -G "Unix Makefiles" "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../Box2D-master/Box2D/')
	aceutils.call(r'make')
else:
	aceutils.call(r'cmake -G "Unix Makefiles" ../Box2D-master/Box2D/')
	aceutils.call(r'make')

aceutils.cd(r"../")


aceutils.cd(r"box2d_bin_x64")

if aceutils.isWin():
	aceutils.call(aceutils.cmd_cmake_x64+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D BOX2D_BUILD_EXAMPLES:BOOL=OFF ../Box2D-master/Box2D/')
	aceutils.call(aceutils.cmd_compile + r'Box2D.sln /p:configuration=Debug')
	aceutils.call(aceutils.cmd_compile + r'Box2D.sln /p:configuration=Release')

aceutils.cd(r"../")

if aceutils.isWin():
	aceutils.mkdir(r'Dev/lib/Debug')
	aceutils.mkdir(r'Dev/lib/Release')
	aceutils.copytree(r'Box2D-master/Box2D/Box2D', r'Dev/include/Box2D/',ignoreList=shutil.ignore_patterns(r'*.txt',r'*.cpp',r'*.cmake'))
	
	aceutils.copy(r'box2d_bin/Box2D/Debug/Box2D.lib', r'Dev/lib/x86/Debug/')
	aceutils.copy(r'box2d_bin/Box2D/Release/Box2D.lib', r'Dev/lib/x86/Release/')
	aceutils.copy(r'box2d_bin_x64/Box2D/Debug/Box2D.lib', r'Dev/lib/x64/Debug/')
	aceutils.copy(r'box2d_bin_x64/Box2D/Release/Box2D.lib', r'Dev/lib/x64/Release/')

else:
	aceutils.copytree(r'Box2D-master/Box2D/Box2D', r'Dev/include/Box2D/')
	aceutils.copy(r'box2d_bin/Box2D/libBox2D.a', r'Dev/lib/')