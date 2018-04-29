
import aceutils

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):

	aceutils.editCmakeForACE(r'Chipmunk2D/CMakeLists.txt','cp932')

	if aceutils.isWin():
		aceutils.call(aceutils.cmd_compile + r'./Chipmunk2D/msvc/vc14/demo/chipmunk.sln /p:configuration=Debug /p:platform=Win32')
		aceutils.call(aceutils.cmd_compile + r'./Chipmunk2D/msvc/vc14/demo/chipmunk.sln /p:configuration=Release /p:platform=Win32')
		aceutils.call(aceutils.cmd_compile + r'./Chipmunk2D/msvc/vc14/demo/chipmunk.sln /p:configuration=Debug /p:platform=x64')
		aceutils.call(aceutils.cmd_compile + r'./Chipmunk2D/msvc/vc14/demo/chipmunk.sln /p:configuration=Release /p:platform=x64')
	elif aceutils.isMac():
		aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release ./Chipmunk2D/')
		aceutils.call(r'make')
	else:
		aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release ./Chipmunk2D/')
		aceutils.call(r'make')

	if aceutils.isWin():
		aceutils.mkdir(r'../Dev/lib/x86/')
		aceutils.mkdir(r'../Dev/lib/x86/Debug')
		aceutils.mkdir(r'../Dev/lib/x86/Release')

		aceutils.mkdir(r'../Dev/lib/x64/')
		aceutils.mkdir(r'../Dev/lib/x64/Debug')
		aceutils.mkdir(r'../Dev/lib/x64/Release')

		aceutils.copytree(r'./Chipmunk2D/include/chipmunk/', r'../Dev/include/chipmunk/')

		aceutils.copy(r'Chipmunk2D/msvc/vc14/chipmunk/Win32/Debug/chipmunk.lib', r'../Dev/lib/x86/Debug/')
		aceutils.copy(r'Chipmunk2D/msvc/vc14/chipmunk/Win32/Release/chipmunk.lib', r'../Dev/lib/x86/Release/')
		aceutils.copy(r'Chipmunk2D/msvc/vc14/chipmunk/x64/Debug/chipmunk.lib', r'../Dev/lib/x64/Debug/')
		aceutils.copy(r'Chipmunk2D/msvc/vc14/chipmunk/x64/Release/chipmunk.lib', r'../Dev/lib/x64/Release/')

	else:
		aceutils.copytree(r'chipmunk2d/include/', r'../Dev/include/')
		aceutils.copy(r'chipmunk2d_bin/libchipmunk.a', r'../Dev/lib/')