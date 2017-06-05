
import aceutils
import sys

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):

	# Build dependencies
	if not aceutils.isMac():
		aceutils.call(sys.executable + ' AltseedRHI/Script/build_glew.py')

	# Comple
	aceutils.rmdir(r"AltseedRHI_bin")
	aceutils.rmdir(r"AltseedRHI_bin_x64")
	
	aceutils.mkdir(r"AltseedRHI_bin")
	aceutils.mkdir(r"AltseedRHI_bin_x64")

	with aceutils.CurrentDir('AltseedRHI_bin'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ../AltseedRHI/')
			aceutils.call(aceutils.cmd_compile + r'AltseedRHI.sln /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'AltseedRHI.sln /p:configuration=Release')
		elif aceutils.isMac():
			aceutils.call(r'cmake -G "Unix Makefiles" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../AltseedRHI/')
			aceutils.call(r'make')
		else:
			aceutils.call(r'cmake -G "Unix Makefiles" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ../AltseedRHI/')
			aceutils.call(r'make')

	with aceutils.CurrentDir('AltseedRHI_bin_x64'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake_x64+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ../AltseedRHI/')
			aceutils.call(aceutils.cmd_compile + r'AltseedRHI.sln /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'AltseedRHI.sln /p:configuration=Release')

	dstIncPath = r'../Dev/include/'
	dstLibPath = r'../Dev/lib/'

	aceutils.mkdir(dstIncPath)
	aceutils.mkdir(dstLibPath)
	aceutils.copytreeWithExt(r'AltseedRHI/include/', dstIncPath,['.h'])
	if not aceutils.isMac():
		aceutils.copytreeWithExt(r'AltseedRHI/lib/', dstLibPath,['.lib','.a'])


	if aceutils.isWin():
		aceutils.mkdir(dstLibPath + r'x86/')
		aceutils.mkdir(dstLibPath + r'x86/Debug')
		aceutils.mkdir(dstLibPath + r'x86/Release')

		aceutils.mkdir(dstLibPath + r'x64/')
		aceutils.mkdir(dstLibPath + r'x64/Debug')
		aceutils.mkdir(dstLibPath + r'x64/Release')

		aceutils.copy(r'AltseedRHI_bin/src/Debug/AltseedRHI.lib', dstLibPath + r'x86/Debug/')
		aceutils.copy(r'AltseedRHI_bin/src/Release/AltseedRHI.lib', dstLibPath + r'x86/Release/')
		aceutils.copy(r'AltseedRHI_bin_x64/src/Debug/AltseedRHI.lib', dstLibPath + r'x64/Debug/')
		aceutils.copy(r'AltseedRHI_bin_x64/src/Release/AltseedRHI.lib', dstLibPath + r'x64/Release/')

	else:
		aceutils.copy(r'AltseedRHI_bin/src/libAltseedRHI.a', dstLibPath)
