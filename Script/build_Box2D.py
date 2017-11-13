
import aceutils
import shutil
import os

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):

    aceutils.rmdir(r"box2d_temp")

    if aceutils.isWin() or aceutils.isMac():
        aceutils.copytree(r"Box2D", "box2d_temp")
    else:
        aceutils.copytree(r"Box2D-Linux", "box2d_temp")

    aceutils.rmdir(r"box2d_temp/.git/")

    aceutils.rmdir(r"box2d_bin")
    
    if aceutils.isWin() or aceutils.isMac():
        print("Not edit")
    else:
        aceutils.editCmakeForACE(r'box2d_temp/Box2D/CMakeLists.txt','cp932')
        aceutils.editCmakeForACE(r'box2d_temp/Box2D/Box2D/CMakeLists.txt','cp932')
    aceutils.mkdir(r"box2d_bin")

    pathname = r"box2d_temp/"

    if aceutils.isWin():
         with aceutils.CurrentDir("box2d_temp/Box2D"):
             aceutils.copy('../../../Script/premake5/windows/premake5.lua.box2d','premake5.lua.box2d')
             aceutils.call(aceutils.cmd_premake5 + r'--file=' + 'premake5.lua.box2d')
             with aceutils.CurrentDir(r'Build/vs2015'):
                 aceutils.call(aceutils.cmd_compile + r'Box2D.sln /p:configuration=Debug /p:outdir=Debug_x86 /p:platform=Win32 /p:RuntimeLibrary=MTd_StaticDebug')
                 aceutils.call(aceutils.cmd_compile + r'Box2D.sln /p:configuration=Release /p:outdir=Release_x86 /p:platform=Win32 /p:RuntimeLibrary=MT_StaticRelease')
                 aceutils.call(aceutils.cmd_compile + r'Box2D.sln /p:configuration=Debug /p:outdir=Debug_x64 /p:platform=x64 /p:RuntimeLibrary=MTd_StaticDebug')
                 aceutils.call(aceutils.cmd_compile + r'Box2D.sln /p:configuration=Release /p:outdir=Release_x64 /p:platform=x64 /p:RuntimeLibrary=MT_StaticRelease')

    elif aceutils.isMac():
        with aceutils.CurrentDir("box2d_temp/Box2D"):
                aceutils.call(aceutils.cmd_premake5 + r'xcode4')
                with aceutils.CurrentDir(r'Build/xcode4'):
                    aceutils.call(r'xcodebuild -project Box2D.xcodeproj ONLY_ACTIVE_ARCH=NO ARCHS="i386 x86_64" -configuration Release')
    else:
        with aceutils.CurrentDir("box2d_bin"):
            aceutils.call(r'cmake -G "Unix Makefiles" ../' + pathname + r'/Box2D/')
            aceutils.call(r'make')

    if aceutils.isWin():
        aceutils.mkdir(r'../Dev/lib/Debug')
        aceutils.mkdir(r'../Dev/lib/Release')
        aceutils.copytree(pathname + r'/Box2D/Box2D', r'../Dev/include/Box2D/', True, ignoreList=shutil.ignore_patterns(r'*.txt',r'*.cpp',r'*.cmake'))
        
        aceutils.copy(r'box2d_temp/Box2D/Build/vs2015/Debug_x86/Box2D.lib', r'../Dev/lib/x86/Debug/')
        aceutils.copy(r'box2d_temp/Box2D/Build/vs2015/Release_x86/Box2D.lib', r'../Dev/lib/x86/Release/')
        aceutils.copy(r'box2d_temp/Box2D/Build/vs2015/Debug_x64/Box2D.lib', r'../Dev/lib/x64/Debug/')
        aceutils.copy(r'box2d_temp/Box2D/Build/vs2015/Release_x64/Box2D.lib', r'../Dev/lib/x64/Release/')

    elif aceutils.isMac():
        aceutils.copytree(pathname + r'/Box2D/Box2D', r'../Dev/include/Box2D/', True)
        aceutils.copy(r'Box2D/Box2D/Build/xcode4/bin/Release/libBox2D.a', r'../Dev/lib/')
        # ダミー用にコピー
        aceutils.mkdir(r'box2d_bin/Box2D')
        aceutils.copy(r'Box2D/Box2D/Build/xcode4/bin/Release/libBox2D.a', r'box2d_bin/Box2D/')
    else:
        aceutils.copytree(pathname + r'/Box2D/Box2D', r'../Dev/include/Box2D/', True)
        aceutils.copy(r'box2d_bin/Box2D/libBox2D.a', r'../Dev/lib/')
