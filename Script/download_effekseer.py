
import aceutils

aceutils.cdToScript()
aceutils.cd(r'../')
aceutils.rm(r'master.zip')
aceutils.rmdir(r'Effekseer-master')

aceutils.wget(r'https://github.com/effekseer/Effekseer/archive/master.zip')
aceutils.unzip(r'master.zip')

aceutils.editCmakeForACE(r'Effekseer-master/Dev/Cpp/CMakeLists.txt','cp932')

aceutils.mkdir(r"effekseer_bin")
aceutils.cd(r"effekseer_bin")

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../Effekseer-master/Dev/Cpp/')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Effekseer.sln /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Effekseer.sln /p:configuration=Release')
elif aceutils.isMac():
	aceutils.call(r'cmake -G "Unix Makefiles" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF -D USE_GLEW_STATIC:BOOL=OFF -D USE_GLEW_DLL:BOOL=OFF -D USE_OPENAL:BOOL=OFF ../Effekseer-master/Dev/Cpp/')
	aceutils.call(r'make')
else:
	aceutils.call(r'cmake -G "Unix Makefiles" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF -D USE_OPENAL:BOOL=OFF ../Effekseer-master/Dev/Cpp/')
	aceutils.call(r'make')

aceutils.cd(r"../")

if aceutils.isWin():
	aceutils.mkdir(r'Dev/lib/Debug')
	aceutils.mkdir(r'Dev/lib/Release')
	aceutils.copy(r'Effekseer-master/Dev/Cpp/Effekseer/Effekseer.h', r'Dev/include/')
	aceutils.copy(r'Effekseer-master/Dev/Cpp/EffekseerRendererDX11/EffekseerRendererDX11.h', r'Dev/include/')
	aceutils.copy(r'Effekseer-master/Dev/Cpp/EffekseerRendererGL/EffekseerRendererGL.h', r'Dev/include/')
	aceutils.copy(r'effekseer_bin/Debug/Effekseer.lib', r'Dev/lib/Debug/')
	aceutils.copy(r'effekseer_bin/Debug/EffekseerRendererDX11.lib', r'Dev/lib/Debug/')
	aceutils.copy(r'effekseer_bin/Debug/EffekseerRendererGL.lib', r'Dev/lib/Debug/')
	aceutils.copy(r'effekseer_bin/Release/Effekseer.lib', r'Dev/lib/Release/')
	aceutils.copy(r'effekseer_bin/Release/EffekseerRendererDX11.lib', r'Dev/lib/Release/')
	aceutils.copy(r'effekseer_bin/Release/EffekseerRendererGL.lib', r'Dev/lib/Release/')
else:
	aceutils.copy(r'Effekseer-master/Dev/Cpp/Effekseer/Effekseer.h', r'Dev/include/')
	aceutils.copy(r'Effekseer-master/Dev/Cpp/EffekseerRendererGL/EffekseerRendererGL.h', r'Dev/include/')
	aceutils.copy(r'effekseer_bin/libEffekseer.a', r'Dev/lib/')
	aceutils.copy(r'effekseer_bin/libEffekseerRendererGL.a', r'Dev/lib/')


