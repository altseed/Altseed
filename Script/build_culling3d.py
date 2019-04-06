
import aceutils

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):
	aceutils.rmdir(r"culling3d_bin")
	aceutils.rmdir(r"culling3d_bin_x64")

	aceutils.mkdir(r"culling3d_bin")
	aceutils.mkdir(r"culling3d_bin_x64")

	with aceutils.CurrentDir('culling3d_bin'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ../Culling3D/')
			aceutils.call('cmake --build . --config Debug')
			aceutils.call('cmake --build . --config Release')
		elif aceutils.isMac():
			aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF "-DCMAKE_OSX_ARCHITECTURES=x86_64' + (';i386' if aceutils.Isi386() else '') + r'" ../Culling3D/')
			aceutils.call(r'make')
		else:
			aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ../Culling3D/')
			aceutils.call(r'make')

	with aceutils.CurrentDir('culling3d_bin_x64'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake_x64+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ../Culling3D/')
			aceutils.call('cmake --build . --config Debug')
			aceutils.call('cmake --build . --config Release')

	if aceutils.isWin():
		aceutils.mkdir(r'../Dev/lib/x86/')
		aceutils.mkdir(r'../Dev/lib/x86/Debug')
		aceutils.mkdir(r'../Dev/lib/x86/Release')

		aceutils.mkdir(r'../Dev/lib/x64/')
		aceutils.mkdir(r'../Dev/lib/x64/Debug')
		aceutils.mkdir(r'../Dev/lib/x64/Release')

		aceutils.copy(r'Culling3D/Culling3D/Culling3D.h', r'../Dev/include/')
		
		aceutils.copy(r'culling3d_bin/Debug/Culling3D.lib', r'../Dev/lib/x86/Debug/')
		aceutils.copy(r'culling3d_bin/Release/Culling3D.lib', r'../Dev/lib/x86/Release/')

		aceutils.copy(r'culling3d_bin_x64/Debug/Culling3D.lib', r'../Dev/lib/x64/Debug/')
		aceutils.copy(r'culling3d_bin_x64/Release/Culling3D.lib', r'../Dev/lib/x64/Release/')

	else:
		aceutils.copy(r'Culling3D/Culling3D/Culling3D.h', r'../Dev/include/')
		aceutils.copy(r'culling3d_bin/libCulling3D.a', r'../Dev/lib/')
