
import aceutils

aceutils.cdToScript()
aceutils.cd(r'../')
aceutils.rm(r'master.zip')
aceutils.rmdir(r"OpenSoundMixer-master")

aceutils.wget(r'https://github.com/altseed/OpenSoundMixer/archive/master.zip')
aceutils.unzip(r'master.zip')

aceutils.editCmakeForACE(r'OpenSoundMixer-master/CMakeLists.txt','cp932')


aceutils.rmdir(r"osm_bin")
aceutils.rmdir(r"osm_bin_x64")

aceutils.mkdir(r"osm_bin")
aceutils.mkdir(r"osm_bin_x64")

aceutils.cd(r"osm_bin")

if aceutils.isWin():
	aceutils.call(aceutils.cmd_cmake+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../OpenSoundMixer-master/')
	aceutils.call(aceutils.cmd_compile + r'OpenSoundMixer.sln /p:configuration=Debug')
	aceutils.call(aceutils.cmd_compile + r'OpenSoundMixer.sln /p:configuration=Release')
elif aceutils.isMac():
	aceutils.call(r'cmake -G "Unix Makefiles" "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../OpenSoundMixer-master/')
	aceutils.call(r'make')
else:
	aceutils.call(r'cmake -G "Unix Makefiles" ../OpenSoundMixer-master/')
	aceutils.call(r'make')

aceutils.cd(r"../")


aceutils.cd(r"osm_bin_x64")

if aceutils.isWin():
	aceutils.call(aceutils.cmd_cmake+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../OpenSoundMixer-master/')
	aceutils.call(aceutils.cmd_compile + r'OpenSoundMixer.sln /p:configuration=Debug')
	aceutils.call(aceutils.cmd_compile + r'OpenSoundMixer.sln /p:configuration=Release')

aceutils.cd(r"../")

if aceutils.isWin():
	aceutils.copy(r'OpenSoundMixer-master/src/OpenSoundMixer.h', r'Dev/include/')

	aceutils.copy(r'osm_bin/Debug/OpenSoundMixer.lib', r'Dev/lib/x86/Debug/')
	aceutils.copy(r'osm_bin/Release/OpenSoundMixer.lib', r'Dev/lib/x86/Release/')

	aceutils.copy(r'OpenSoundMixer-master/lib/x86/Debug/VS2013/libogg_static.lib', r'Dev/lib/x86/Debug/VS2013/')
	aceutils.copy(r'OpenSoundMixer-master/lib/x86/Debug/VS2013/libvorbis_static.lib', r'Dev/lib/x86/Debug/VS2013/')
	aceutils.copy(r'OpenSoundMixer-master/lib/x86/Debug/VS2013/libvorbisfile_static.lib', r'Dev/lib/x86/Debug/VS2013/')
	
	aceutils.copy(r'OpenSoundMixer-master/lib/x86/Release/VS2013/libogg_static.lib', r'Dev/lib/x86/Release/VS2013/')
	aceutils.copy(r'OpenSoundMixer-master/lib/x86/Release/VS2013/libvorbis_static.lib', r'Dev/lib/x86/Release/VS2013/')
	aceutils.copy(r'OpenSoundMixer-master/lib/x86/Release/VS2013/libvorbisfile_static.lib', r'Dev/lib/x86/Release/VS2013/')

	aceutils.copy(r'OpenSoundMixer-master/lib/x86/Debug/VS2015/libogg_static.lib', r'Dev/lib/x86/Debug/VS2015/')
	aceutils.copy(r'OpenSoundMixer-master/lib/x86/Debug/VS2015/libvorbis_static.lib', r'Dev/lib/x86/Debug/VS2015/')
	aceutils.copy(r'OpenSoundMixer-master/lib/x86/Debug/VS2015/libvorbisfile_static.lib', r'Dev/lib/x86/Debug/VS2015/')
	
	aceutils.copy(r'OpenSoundMixer-master/lib/x86/Release/VS2015/libogg_static.lib', r'Dev/lib/x86/Release/VS2015/')
	aceutils.copy(r'OpenSoundMixer-master/lib/x86/Release/VS2015/libvorbis_static.lib', r'Dev/lib/x86/Release/VS2015/')
	aceutils.copy(r'OpenSoundMixer-master/lib/x86/Release/VS2015/libvorbisfile_static.lib', r'Dev/lib/x86/Release/VS2015/')

	aceutils.copy(r'osm_bin_x64/Debug/OpenSoundMixer.lib', r'Dev/lib/x64/Debug/')
	aceutils.copy(r'osm_bin_x64/Release/OpenSoundMixer.lib', r'Dev/lib/x64/Release/')

	aceutils.copy(r'OpenSoundMixer-master/lib/x64/Debug/VS2013/libogg_static.lib', r'Dev/lib/x64/Debug/VS2013/')
	aceutils.copy(r'OpenSoundMixer-master/lib/x64/Debug/VS2013/libvorbis_static.lib', r'Dev/lib/x64/Debug/VS2013/')
	aceutils.copy(r'OpenSoundMixer-master/lib/x64/Debug/VS2013/libvorbisfile_static.lib', r'Dev/lib/x64/Debug/VS2013/')
	
	aceutils.copy(r'OpenSoundMixer-master/lib/x64/Release/VS2013/libogg_static.lib', r'Dev/lib/x64/Release/VS2013/')
	aceutils.copy(r'OpenSoundMixer-master/lib/x64/Release/VS2013/libvorbis_static.lib', r'Dev/lib/x64/Release/VS2013/')
	aceutils.copy(r'OpenSoundMixer-master/lib/x64/Release/VS2013/libvorbisfile_static.lib', r'Dev/lib/x64/Release/VS2013/')

	aceutils.copy(r'OpenSoundMixer-master/lib/x64/Debug/VS2015/libogg_static.lib', r'Dev/lib/x64/Debug/VS2015/')
	aceutils.copy(r'OpenSoundMixer-master/lib/x64/Debug/VS2015/libvorbis_static.lib', r'Dev/lib/x64/Debug/VS2015/')
	aceutils.copy(r'OpenSoundMixer-master/lib/x64/Debug/VS2015/libvorbisfile_static.lib', r'Dev/lib/x64/Debug/VS2015/')
	
	aceutils.copy(r'OpenSoundMixer-master/lib/x64/Release/VS2015/libogg_static.lib', r'Dev/lib/x64/Release/VS2015/')
	aceutils.copy(r'OpenSoundMixer-master/lib/x64/Release/VS2015/libvorbis_static.lib', r'Dev/lib/x64/Release/VS2015/')
	aceutils.copy(r'OpenSoundMixer-master/lib/x64/Release/VS2015/libvorbisfile_static.lib', r'Dev/lib/x64/Release/VS2015/')

else:
	aceutils.copy(r'OpenSoundMixer-master/src/OpenSoundMixer.h', r'Dev/include/')
	aceutils.copy(r'osm_bin/lib/libOpenSoundMixer.a', r'Dev/lib/')
