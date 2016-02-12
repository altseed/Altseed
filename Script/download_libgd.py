
import aceutils

zipname = r'fc14a8c1bb22542db00858a04495413f0c290001.zip'
pathname = r'libgd-fc14a8c1bb22542db00858a04495413f0c290001'

aceutils.cdToScript()
aceutils.cd(r'../')
aceutils.rm(zipname)
aceutils.rmdir(pathname)

aceutils.wget(r'https://github.com/libgd/libgd/archive/' + zipname)
aceutils.unzip(zipname)

aceutils.rmdir(r"gd_bin")
aceutils.rmdir(r"gd_bin_x64")

aceutils.mkdir(r"gd_bin")
aceutils.mkdir(r"gd_bin_x64")

aceutils.editCmakeForACE(pathname + r'/CMakeLists.txt','cp932')
aceutils.editCmakeForACE(pathname + r'/src/CMakeLists.txt','cp932')

aceutils.cd(r"gd_bin")

if aceutils.isWin():
	aceutils.call(aceutils.cmd_cmake + r'../' + pathname + '/')
	aceutils.call(aceutils.cmd_compile + r'src/libgd_static.vcxproj /p:configuration=Debug')
	aceutils.call(aceutils.cmd_compile + r'src/libgd_static.vcxproj /p:configuration=Release')
elif aceutils.isMac():
	aceutils.call(r'cmake -G "Unix Makefiles" "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../' + pathname + '/')
	aceutils.call(r'make')
else:
	aceutils.call(r'cmake -G "Unix Makefiles" ../' + pathname + '/')
	aceutils.call(r'make')

aceutils.cd(r"../")

aceutils.cd(r"gd_bin_x64")

if aceutils.isWin():
	aceutils.call(aceutils.cmd_cmake_x64 + r'../' + pathname + '/')
	aceutils.call(aceutils.cmd_compile + r'src/libgd_static.vcxproj /p:configuration=Debug')
	aceutils.call(aceutils.cmd_compile + r'src/libgd_static.vcxproj /p:configuration=Release')

aceutils.cd(r"../")

if aceutils.isWin():
	aceutils.mkdir(r'Dev/lib/x86/')
	aceutils.mkdir(r'Dev/lib/x86/Debug')
	aceutils.mkdir(r'Dev/lib/x86/Release')

	aceutils.mkdir(r'Dev/lib/x64/')
	aceutils.mkdir(r'Dev/lib/x64/Debug')
	aceutils.mkdir(r'Dev/lib/x64/Release')

	aceutils.mkdir(r'Dev/include/gd')

	aceutils.copytreeWithExt(pathname + r'/src/', r'Dev/include/gd/',['.h'])

	aceutils.copy(r'gd_bin/Bin/Debug/libgd_static.lib', r'Dev/lib/x86/Debug/')
	aceutils.copy(r'gd_bin/Bin/Release/libgd_static.lib', r'Dev/lib/x86/Release/')
	aceutils.copy(r'gd_bin_x64/Bin/Debug/libgd_static.lib', r'Dev/lib/x64/Debug/')
	aceutils.copy(r'gd_bin_x64/Bin/Release/libgd_static.lib', r'Dev/lib/x64/Release/')

else:
	aceutils.copytreeWithExt(r'libgd-gd-2.1.1/src/', r'Dev/include/gd/',['.h'])
	aceutils.copy(r'gd_bin/Bin/libgd.a', r'Dev/lib/')


