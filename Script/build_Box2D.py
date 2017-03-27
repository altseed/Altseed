
import aceutils
import shutil

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):
    aceutils.rmdir(r"box2d_bin")
    aceutils.rmdir(r"box2d_bin_x64")

    aceutils.editCmakeForACE(r'Box2D/Box2D/CMakeLists.txt','cp932')
    aceutils.editCmakeForACE(r'Box2D-Linux/Box2D/CMakeLists.txt','cp932')
    aceutils.editCmakeForACE(r'Box2D/Box2D/Box2D/CMakeLists.txt','cp932')
    aceutils.editCmakeForACE(r'Box2D-Linux/Box2D/Box2D/CMakeLists.txt','cp932')
    aceutils.mkdir(r"box2d_bin")
    aceutils.mkdir(r"box2d_bin_x64")

    if aceutils.isWin():
        pathname = r"Box2D"
    elif aceutils.isMac():
        pathname = r"Box2D"
    else:
        pathname = r"Box2D-Linux"

    with aceutils.CurrentDir("box2d_bin"):
        if aceutils.isWin():
            aceutils.call(aceutils.cmd_cmake+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D BOX2D_BUILD_EXAMPLES:BOOL=OFF ../' + pathname + r'/Box2D/')
            aceutils.call(aceutils.cmd_compile + r'Box2D.sln /p:configuration=Debug')
            aceutils.call(aceutils.cmd_compile + r'Box2D.sln /p:configuration=Release')
        elif aceutils.isMac():
            aceutils.call(r'cmake -G "Unix Makefiles" "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../' + pathname + r'/Box2D/')
            aceutils.call(r'make')
        else:
            aceutils.call(r'cmake -G "Unix Makefiles" ../' + pathname + r'/Box2D/')
            aceutils.call(r'make')

    with aceutils.CurrentDir("box2d_bin_x64"):
        if aceutils.isWin():
            aceutils.call(aceutils.cmd_cmake_x64+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D BOX2D_BUILD_EXAMPLES:BOOL=OFF ../' + pathname + r'/Box2D/')
            aceutils.call(aceutils.cmd_compile + r'Box2D.sln /p:configuration=Debug')
            aceutils.call(aceutils.cmd_compile + r'Box2D.sln /p:configuration=Release')

    if aceutils.isWin():
        aceutils.mkdir(r'../Dev/lib/Debug')
        aceutils.mkdir(r'../Dev/lib/Release')
        aceutils.copytree(pathname + r'/Box2D/Box2D', r'../Dev/include/Box2D/', True, ignoreList=shutil.ignore_patterns(r'*.txt',r'*.cpp',r'*.cmake'))
        
        aceutils.copy(r'box2d_bin/Box2D/Debug/Box2D.lib', r'../Dev/lib/x86/Debug/')
        aceutils.copy(r'box2d_bin/Box2D/Release/Box2D.lib', r'../Dev/lib/x86/Release/')
        aceutils.copy(r'box2d_bin_x64/Box2D/Debug/Box2D.lib', r'../Dev/lib/x64/Debug/')
        aceutils.copy(r'box2d_bin_x64/Box2D/Release/Box2D.lib', r'../Dev/lib/x64/Release/')
    else:
        aceutils.copytree(pathname + r'/Box2D/Box2D', r'../Dev/include/Box2D/', True)
        aceutils.copy(r'box2d_bin/Box2D/libBox2D.a', r'../Dev/lib/')
