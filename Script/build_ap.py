
import aceutils
import sys

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):

	# Build dependencies
	aceutils.call(sys.executable + ' AltseedPlatform/Script/build_glfw.py')

	# Comple
	aceutils.rmdir(r"AltseedPlatform_bin")
	aceutils.rmdir(r"AltseedPlatform_bin_x64")
	
	aceutils.mkdir(r"AltseedPlatform_bin")
	aceutils.mkdir(r"AltseedPlatform_bin_x64")

	with aceutils.CurrentDir('AltseedPlatform_bin'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ../AltseedPlatform/')
			aceutils.call(aceutils.cmd_compile + r'AltseedPlatform.sln /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'AltseedPlatform.sln /p:configuration=Release')
		elif aceutils.isMac():
			aceutils.call(r'cmake -G "Unix Makefiles" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../AltseedPlatform/')
			aceutils.call(r'make')
		else:
			aceutils.call(r'cmake -G "Unix Makefiles" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ../AltseedPlatform/')
			aceutils.call(r'make')

	with aceutils.CurrentDir('AltseedPlatform_bin_x64'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake_x64+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ../AltseedPlatform/')
			aceutils.call(aceutils.cmd_compile + r'AltseedPlatform.sln /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'AltseedPlatform.sln /p:configuration=Release')

	dstIncPath = r'../Dev/include/'
	dstLibPath = r'../Dev/lib/'

	aceutils.mkdir(dstIncPath)
	aceutils.mkdir(dstLibPath)
	aceutils.copytreeWithExt(r'AltseedPlatform/include/', dstIncPath,['.h'])
	aceutils.copytreeWithExt(r'AltseedPlatform/lib/', dstLibPath,['.lib','.a'])


	if aceutils.isWin():
		aceutils.mkdir(dstLibPath + r'x86/')
		aceutils.mkdir(dstLibPath + r'x86/Debug')
		aceutils.mkdir(dstLibPath + r'x86/Release')

		aceutils.mkdir(dstLibPath + r'x64/')
		aceutils.mkdir(dstLibPath + r'x64/Debug')
		aceutils.mkdir(dstLibPath + r'x64/Release')

		aceutils.copy(r'AltseedPlatform_bin/src/Debug/AltseedPlatform.lib', dstLibPath + r'x86/Debug/')
		aceutils.copy(r'AltseedPlatform_bin/src/Release/AltseedPlatform.lib', dstLibPath + r'x86/Release/')
		aceutils.copy(r'AltseedPlatform_bin_x64/src/Debug/AltseedPlatform.lib', dstLibPath + r'x64/Debug/')
		aceutils.copy(r'AltseedPlatform_bin_x64/src/Release/AltseedPlatform.lib', dstLibPath + r'x64/Release/')

	else:
		aceutils.copy(r'AltseedPlatform_bin/src/libAltseedPlatform.a', dstLibPath)
