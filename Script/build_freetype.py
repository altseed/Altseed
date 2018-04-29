
import aceutils

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):
	aceutils.rmdir(r"freetype_bin")
	aceutils.rmdir(r"freetype_bin_x64")

	aceutils.wget(r'https://sourceforge.net/projects/freetype/files/freetype2/2.6/ft26.zip') 
	aceutils.mkdir(r"freetype_bin")
	aceutils.mkdir(r"freetype_bin_x64")
	aceutils.editCmakeForACE(r'freetype/CMakeLists.txt','cp932')

	with aceutils.CurrentDir('freetype_bin'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake+r'../freetype/')
			aceutils.call(aceutils.cmd_compile + r'freetype.sln /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'freetype.sln /p:configuration=Release')
		elif aceutils.isMac():
			aceutils.call(r'cmake -G "Unix Makefiles" "-DWITH_BZip2=OFF" "-DWITH_HarfBuzz=OFF" "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" -D CMAKE_BUILD_TYPE=Release ../freetype/')
			aceutils.call(r'make')
		else:
			aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release ../freetype/')
			aceutils.call(r'make')

	with aceutils.CurrentDir('freetype_bin_x64'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake_x64+r'../freetype/')
			aceutils.call(aceutils.cmd_compile + r'freetype.sln /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'freetype.sln /p:configuration=Release')

	if aceutils.isWin():
		aceutils.mkdir(r'../Dev/lib/x86/')
		aceutils.mkdir(r'../Dev/lib/x86/Debug')
		aceutils.mkdir(r'../Dev/lib/x86/Release')

		aceutils.mkdir(r'../Dev/lib/x64/')
		aceutils.mkdir(r'../Dev/lib/x64/Debug')
		aceutils.mkdir(r'../Dev/lib/x64/Release')

		aceutils.copytree(r'freetype/include/', r'../Dev/include/freetype')

		aceutils.copy(r'freetype_bin/Debug/freetyped.lib', r'../Dev/lib/x86/Debug/freetype.lib')
		aceutils.copy(r'freetype_bin/Release/freetype.lib', r'../Dev/lib/x86/Release/')
		aceutils.copy(r'freetype_bin_x64/Debug/freetyped.lib', r'../Dev/lib/x64/Debug/freetype.lib')
		aceutils.copy(r'freetype_bin_x64/Release/freetype.lib', r'../Dev/lib/x64/Release/')

	else:
		aceutils.copytree(r'freetype/include/', r'../Dev/include/freetype')
		aceutils.copy(r'freetype_bin/libfreetype.a', r'../Dev/lib/')
