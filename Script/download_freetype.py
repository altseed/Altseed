
import aceutils

aceutils.cdToScript()
aceutils.cd(r'../')
aceutils.rm(r'ft26.zip')
aceutils.rmdir(r'freetype-2.6')

aceutils.wget(r'http://sourceforge.net/projects/freetype/files/freetype2/2.6/ft26.zip')
aceutils.unzip(r'ft26.zip')

aceutils.rmdir(r"freetype_bin")
aceutils.rmdir(r"freetype_bin_x64")

aceutils.mkdir(r"freetype_bin")
aceutils.mkdir(r"freetype_bin_x64")

aceutils.editCmakeForACE(r'freetype-2.6/CMakeLists.txt','cp932')

aceutils.cd(r"freetype_bin")

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12" ../freetype-2.6/')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" freetype.sln /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" freetype.sln /p:configuration=Release')
elif aceutils.isMac():
	aceutils.call(r'cmake -G "Unix Makefiles" "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../freetype-2.6/')
	aceutils.call(r'make')
else:
	aceutils.call(r'cmake -G "Unix Makefiles" ../freetype-2.6/')
	aceutils.call(r'make')

aceutils.cd(r"../")

aceutils.cd(r"freetype_bin_x64")

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12 Win64" ../freetype-2.6/')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" freetype.sln /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" freetype.sln /p:configuration=Release')

aceutils.cd(r"../")

if aceutils.isWin():
	aceutils.mkdir(r'Dev/lib/x86/')
	aceutils.mkdir(r'Dev/lib/x86/Debug')
	aceutils.mkdir(r'Dev/lib/x86/Release')

	aceutils.mkdir(r'Dev/lib/x64/')
	aceutils.mkdir(r'Dev/lib/x64/Debug')
	aceutils.mkdir(r'Dev/lib/x64/Release')

	aceutils.copytree(r'freetype-2.6/include/', r'Dev/include/freetype')

	aceutils.copy(r'freetype_bin/Debug/freetype.lib', r'Dev/lib/x86/Debug/')
	aceutils.copy(r'freetype_bin/Release/freetype.lib', r'Dev/lib/x86/Release/')
	aceutils.copy(r'freetype_bin_x64/Debug/freetype.lib', r'Dev/lib/x64/Debug/')
	aceutils.copy(r'freetype_bin_x64/Release/freetype.lib', r'Dev/lib/x64/Release/')

else:
	aceutils.copytree(r'freetype-2.6/include/', r'Dev/include/freetype')
	aceutils.copy(r'freetype_bin/bin/libfreetype.a', r'Dev/lib/')


