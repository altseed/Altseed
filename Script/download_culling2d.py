
import aceutils

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):
	aceutils.rmdir(r"culling2d_bin")
	aceutils.rmdir(r"culling2d_bin_x64")

	if aceutils.isWin():
		aceutils.editCmakeForACE(r'Culling2D/CMakeLists.txt','cp932')
	elif aceutils.isMac():
		aceutils.editCmakeForACE(r'Culling2D/CMakeLists.txt','cp932')
	# in Linux, using 'cp932' twice on the same file can cause an error "UnicodeDecodeError: 'cp932' codec can't decode byte 0x9a in position 23: illegal multibyte sequence".
	else:
		try: 
			aceutils.editCmakeForACE(r'Culling2D/CMakeLists.txt','cp932')
		except:
			aceutils.editCmakeForACE(r'Culling2D/CMakeLists.txt')
	
	aceutils.mkdir(r"culling2d_bin")
	aceutils.mkdir(r"culling2d_bin_x64")

	with aceutils.CurrentDir('culling2d_bin'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../Culling2D/')
			aceutils.call(aceutils.cmd_compile + r'Culling2D.sln /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'Culling2D.sln /p:configuration=Release')
		elif aceutils.isMac():
			aceutils.call(r'cmake -G "Unix Makefiles" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../Culling2D/')
			aceutils.call(r'make')
		else:
			aceutils.call(r'cmake -G "Unix Makefiles" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../Culling2D/')
			aceutils.call(r'make')

	with aceutils.CurrentDir('culling2d_bin_x64'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake_x64+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../Culling2D/')
			aceutils.call(aceutils.cmd_compile + r'Culling2D.sln /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'Culling2D.sln /p:configuration=Release')

	if aceutils.isWin():
		aceutils.mkdir(r'../Dev/lib/x86/')
		aceutils.mkdir(r'../Dev/lib/x86/Debug')
		aceutils.mkdir(r'../Dev/lib/x86/Release')

		aceutils.mkdir(r'../Dev/lib/x64/')
		aceutils.mkdir(r'../Dev/lib/x64/Debug')
		aceutils.mkdir(r'../Dev/lib/x64/Release')

		aceutils.copy(r'Culling2D/Culling2D/Culling2D.h', r'../Dev/include/')

		aceutils.copy(r'culling2d_bin/Debug/Culling2D_debug.lib', r'../Dev/lib/x86/Debug/Culling2D.lib')
		aceutils.copy(r'culling2d_bin/Release/Culling2D.lib', r'../Dev/lib/x86/Release/')
		aceutils.copy(r'culling2d_bin_x64/Debug/Culling2D_debug.lib', r'../Dev/lib/x64/Debug/Culling2D.lib')
		aceutils.copy(r'culling2d_bin_x64/Release/Culling2D.lib', r'../Dev/lib/x64/Release/')

	else:
		aceutils.copy(r'Culling2D/Culling2D/Culling2D.h', r'../Dev/include/')
		aceutils.copy(r'culling2d_bin/bin/libCulling2D.a', r'../Dev/lib/')
