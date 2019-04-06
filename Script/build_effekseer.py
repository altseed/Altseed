
import aceutils

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):
	aceutils.rmdir(r"effekseer_bin")
	aceutils.rmdir(r"effekseer_bin_x64")

	aceutils.mkdir(r"effekseer_bin")
	aceutils.mkdir(r"effekseer_bin_x64")

	with aceutils.CurrentDir('effekseer_bin'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=ON ../Effekseer/Dev/Cpp/')
			aceutils.call('cmake --build . --config Debug')
			aceutils.call('cmake --build . --config Release')
		elif aceutils.isMac():
			aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF -D USE_GLEW_STATIC:BOOL=OFF -D USE_GLEW_DLL:BOOL=OFF -D USE_OPENGL3:BOOL=ON -D USE_OPENAL:BOOL=OFF "-DCMAKE_OSX_ARCHITECTURES=x86_64' + (';i386' if aceutils.Isi386() else '') + r'" ../Effekseer/Dev/Cpp/')
			aceutils.call(r'make')
		else:
			aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF -D USE_OPENAL:BOOL=OFF ../Effekseer/Dev/Cpp/')
			aceutils.call(r'make')

	with aceutils.CurrentDir('effekseer_bin_x64'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake_x64+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=ON ../Effekseer/Dev/Cpp/')
			aceutils.call('cmake --build . --config Debug')
			aceutils.call('cmake --build . --config Release')

	if aceutils.isWin():
		aceutils.mkdir(r'../Dev/lib/x86/')
		aceutils.mkdir(r'../Dev/lib/x86/Debug')
		aceutils.mkdir(r'../Dev/lib/x86/Release')

		aceutils.mkdir(r'../Dev/lib/x64/')
		aceutils.mkdir(r'../Dev/lib/x64/Debug')
		aceutils.mkdir(r'../Dev/lib/x64/Release')

		aceutils.copy(r'Effekseer/Dev/Cpp/Effekseer/Effekseer.h', r'../Dev/include/')
		aceutils.copy(r'Effekseer/Dev/Cpp/EffekseerRendererDX11/EffekseerRendererDX11.h', r'../Dev/include/')
		aceutils.copy(r'Effekseer/Dev/Cpp/EffekseerRendererGL/EffekseerRendererGL.h', r'../Dev/include/')

		aceutils.copy(r'effekseer_bin/Debug/Effekseer.lib', r'../Dev/lib/x86/Debug/')
		aceutils.copy(r'effekseer_bin/Debug/EffekseerRendererDX11.lib', r'../Dev/lib/x86/Debug/')
		aceutils.copy(r'effekseer_bin/Debug/EffekseerRendererGL.lib', r'../Dev/lib/x86/Debug/')

		aceutils.copy(r'effekseer_bin/Release/Effekseer.lib', r'../Dev/lib/x86/Release/')
		aceutils.copy(r'effekseer_bin/Release/EffekseerRendererDX11.lib', r'../Dev/lib/x86/Release/')
		aceutils.copy(r'effekseer_bin/Release/EffekseerRendererGL.lib', r'../Dev/lib/x86/Release/')

		aceutils.copy(r'effekseer_bin_x64/Debug/Effekseer.lib', r'../Dev/lib/x64/Debug/')
		aceutils.copy(r'effekseer_bin_x64/Debug/EffekseerRendererDX11.lib', r'../Dev/lib/x64/Debug/')
		aceutils.copy(r'effekseer_bin_x64/Debug/EffekseerRendererGL.lib', r'../Dev/lib/x64/Debug/')
		
		aceutils.copy(r'effekseer_bin_x64/Release/Effekseer.lib', r'../Dev/lib/x64/Release/')
		aceutils.copy(r'effekseer_bin_x64/Release/EffekseerRendererDX11.lib', r'../Dev/lib/x64/Release/')
		aceutils.copy(r'effekseer_bin_x64/Release/EffekseerRendererGL.lib', r'../Dev/lib/x64/Release/')

	else:
		aceutils.copy(r'Effekseer/Dev/Cpp/Effekseer/Effekseer.h', r'../Dev/include/')
		aceutils.copy(r'Effekseer/Dev/Cpp/EffekseerRendererGL/EffekseerRendererGL.h', r'../Dev/include/')
		aceutils.copy(r'effekseer_bin/libEffekseer.a', r'../Dev/lib/')
		aceutils.copy(r'effekseer_bin/libEffekseerRendererGL.a', r'../Dev/lib/')
