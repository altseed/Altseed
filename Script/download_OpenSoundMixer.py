
import aceutils
import itertools

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):
	aceutils.rm(r'master.zip')
	aceutils.rmdir(r"OpenSoundMixer-master")
	aceutils.rmdir(r"osm_bin")
	aceutils.rmdir(r"osm_bin_x64")

	aceutils.wget(r'https://github.com/altseed/OpenSoundMixer/archive/master.zip')
	aceutils.unzip(r'master.zip')
	aceutils.editCmakeForACE(r'OpenSoundMixer-master/CMakeLists.txt','cp932')	
	aceutils.mkdir(r"osm_bin")
	aceutils.mkdir(r"osm_bin_x64")

	with aceutils.CurrentDir('osm_bin'):
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

	with aceutils.CurrentDir('osm_bin_x64'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake_x64+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../OpenSoundMixer-master/')
			aceutils.call(aceutils.cmd_compile + r'OpenSoundMixer.sln /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'OpenSoundMixer.sln /p:configuration=Release')

	if aceutils.isWin():
		aceutils.copy(r'OpenSoundMixer-master/src/OpenSoundMixer.h', r'../Dev/include/')

		aceutils.copy(r'osm_bin/Debug/OpenSoundMixer.lib', r'../Dev/lib/x86/Debug/')
		aceutils.copy(r'osm_bin/Release/OpenSoundMixer.lib', r'../Dev/lib/x86/Release/')
		
		aceutils.copy(r'osm_bin_x64/Debug/OpenSoundMixer.lib', r'../Dev/lib/x64/Debug/')
		aceutils.copy(r'osm_bin_x64/Release/OpenSoundMixer.lib', r'../Dev/lib/x64/Release/')

		# 3つのlibを8通りのターゲットに対してコピー
		architectures = ('x86', 'x64')
		configurations = ('Debug', 'Release')
		ides = ('VS2013', 'VS2015')
		files = ('libogg_static', 'libvorbis_static', 'libvorbisfile_static')
		for a, c, i, f in itertools.product(architectures, configurations, ides, files):
			src = r'OpenSoundMixer-master/lib/{0}/{1}/{2}/{3}.lib'.format(a, c, i, f)
			dst = r'../Dev/lib/{0}/{1}/{2}/'.format(a, c, i, f)
			aceutils.copy(src, dst)

	else:
		aceutils.copy(r'OpenSoundMixer-master/src/OpenSoundMixer.h', r'../Dev/include/')
		aceutils.copy(r'osm_bin/lib/libOpenSoundMixer.a', r'../Dev/lib/')
