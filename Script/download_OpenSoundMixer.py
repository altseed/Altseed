
import aceutils

aceutils.cdToScript()
aceutils.cd(r'../')
aceutils.rm(r'master.zip')
aceutils.rmdir(r'OpenSoundMixer-master')

aceutils.wget(r'https://github.com/ac-engine/OpenSoundMixer/archive/master.zip')
aceutils.unzip(r'master.zip')

aceutils.editCmakeForACE(r'OpenSoundMixer-master/CMakeLists.txt','cp932')

if aceutils.isWin():
	aceutils.cd(r"OpenSoundMixer-master")
else:
	aceutils.mkdir(r"osm_bin")
	aceutils.cd(r"osm_bin")

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../Effekseer-master/Dev/Cpp/')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" OpenSoundMixer.sln /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" OpenSoundMixer.sln /p:configuration=Release')
else:
	aceutils.call(r'cmake -G "Unix Makefiles" ../OpenSoundMixer-master/')
	aceutils.call(r'make')

aceutils.cd(r"../")

if aceutils.isWin():
	aceutils.copy(r'OpenSoundMixer-master/src/OpenSoundMixer.h', r'Dev/include/')

	aceutils.copy(r'OpenSoundMixer-master/lib/Debug/OpenSoundMixer.lib', r'Dev/lib/Debug/')
	aceutils.copy(r'OpenSoundMixer-master/lib/Debug/libogg_static.lib', r'Dev/lib/Debug/')
	aceutils.copy(r'OpenSoundMixer-master/lib/Debug/libvorbis_static.lib', r'Dev/lib/Debug/')
	aceutils.copy(r'OpenSoundMixer-master/lib/Debug/libvorbisfile_static.lib', r'Dev/lib/Debug/')

	aceutils.copy(r'OpenSoundMixer-master/lib/Release/OpenSoundMixer.lib', r'Dev/lib/Release/')
	aceutils.copy(r'OpenSoundMixer-master/lib/Release/libogg_static.lib', r'Dev/lib/Release/')
	aceutils.copy(r'OpenSoundMixer-master/lib/Release/libvorbis_static.lib', r'Dev/lib/Release/')
	aceutils.copy(r'OpenSoundMixer-master/lib/Release/libvorbisfile_static.lib', r'Dev/lib/Release/')
else:
	aceutils.copy(r'OpenSoundMixer-master/src/OpenSoundMixer.h', r'Dev/include/')
	aceutils.copy(r'osm_bin/lib/libOpenSoundMixer.a', r'Dev/lib/')
