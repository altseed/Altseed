
import aceutils
import shutil
import os

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):
    aceutils.rmdir(r"box2d_bin")
    aceutils.rmdir(r"box2d_bin_x64")

    #aceutils.editCmakeForACE(r'Box2D/Box2D/CMakeLists.txt','cp932')
    aceutils.editCmakeForACE(r'Box2D-Linux/Box2D/CMakeLists.txt','cp932')
    #aceutils.editCmakeForACE(r'Box2D/Box2D/Box2D/CMakeLists.txt','cp932')
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
            aceutils.cdToScript()
            with aceutils.CurrentDir(r'../Downloads/Box2D/Box2D'):
                aceutils.call(aceutils.cmd_premake5 + r'vs2015 --file=' + '../../../Script/premake5/windows/premake5.lua.box2d')
                with aceutils.CurrentDir(r'Build/vs2015'):
                    aceutils.call(aceutils.cmd_compile + r'Box2D.sln /p:configuration=Debug /p:outdir=Debug_x86 /p:platform=Win32 /p:RuntimeLibrary=MTd_StaticDebug')
                    aceutils.call(aceutils.cmd_compile + r'Box2D.sln /p:configuration=Release /p:outdir=Release_x86 /p:platform=Win32 /p:RuntimeLibrary=MT_StaticRelease')
        elif aceutils.isMac():
            aceutils.cdToScript()
            with aceutils.CurrentDir(r'../Downloads/Box2D/Box2D'):
                aceutils.call(aceutils.cmd_premake5 + r'xcode4')
                with aceutils.CurrentDir(r'Build/xcode4'):
                    aceutils.call(r'xcodebuild -project Box2D.xcodeproj ONLY_ACTIVE_ARCH=NO ARCHS="i386 x86_64" -configuration Release')
        else:
            aceutils.call(r'cmake -G "Unix Makefiles" ../' + pathname + r'/Box2D/')
            aceutils.call(r'make')

    with aceutils.CurrentDir("box2d_bin_x64"):
        if aceutils.isWin():
            aceutils.cdToScript()
            with aceutils.CurrentDir(r'../Downloads/Box2D/Box2D'):
                aceutils.call(aceutils.cmd_premake5 + r'vs2015 --file=' + '../../../Script/premake5/windows/premake5.lua.box2d')
                with aceutils.CurrentDir(r'Build/vs2015'):
                    aceutils.call(aceutils.cmd_compile + r'Box2D.sln /p:configuration=Debug /p:outdir=Debug_x64 /p:platform=x64 /p:RuntimeLibrary=MTd_StaticDebug')
                    aceutils.call(aceutils.cmd_compile + r'Box2D.sln /p:configuration=Release /p:outdir=Release_x64 /p:platform=x64 /p:RuntimeLibrary=MT_StaticRelease')

    if aceutils.isWin():
        aceutils.mkdir(r'../Dev/lib/Debug')
        aceutils.mkdir(r'../Dev/lib/Release')
        aceutils.copytree(pathname + r'/Box2D/Box2D', r'../Dev/include/Box2D/', True, ignoreList=shutil.ignore_patterns(r'*.txt',r'*.cpp',r'*.cmake'))
        
        aceutils.copy(r'Box2D/Box2D/Build/vs2015/Debug_x86/Box2D.lib', r'../Dev/lib/x86/Debug/')
        aceutils.copy(r'Box2D/Box2D/Build/vs2015/Release_x86/Box2D.lib', r'../Dev/lib/x86/Release/')
        aceutils.copy(r'Box2D/Box2D/Build/vs2015/Debug_x64/Box2D.lib', r'../Dev/lib/x64/Debug/')
        aceutils.copy(r'Box2D/Box2D/Build/vs2015/Release_x64/Box2D.lib', r'../Dev/lib/x64/Release/')
    elif aceutils.isMac():
        aceutils.copytree(pathname + r'/Box2D/Box2D', r'../Dev/include/Box2D/', True)
        aceutils.copy(r'Box2D/Box2D/Build/xcode4/bin/Release/libBox2D.a', r'../Dev/lib/')
        # ダミー用にコピー
        aceutils.mkdir(r'box2d_bin/Box2D')
        aceutils.copy(r'Box2D/Box2D/Build/xcode4/bin/Release/libBox2D.a', r'box2d_bin/Box2D/')
    else:
        aceutils.copytree(pathname + r'/Box2D/Box2D', r'../Dev/include/Box2D/', True)
        aceutils.copy(r'box2d_bin/Box2D/libBox2D.a', r'../Dev/lib/')
