import os.path
import aceutils

aceutils.cdToScript()
aceutils.cd(r'../')

aceutils.cd(r'Downloads')

aceutils.rmdir(r'zlib_bin')
aceutils.rmdir(r'libpng_bin')

aceutils.editCmakeForACE(r'zlib/CMakeLists.txt')
aceutils.editCmakeForACE(r'libpng/CMakeLists.txt')

aceutils.mkdir(r'zlib_bin')
aceutils.mkdir(r'libpng_bin')


if aceutils.isWin():
	print('already installed.')
else:
	if aceutils.Isi386():
		aceutils.mkdir(r'zlib_bin/i386')
		aceutils.mkdir(r'libpng_bin/i386')

	aceutils.cd(r'zlib_bin')
	if aceutils.isMac():
		aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX:PATH=../../Dev "-DCMAKE_OSX_ARCHITECTURES=x86_64' + (';i386' if aceutils.Isi386() else '') + r'" ../zlib/')
	else:
		aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX:PATH=../../Dev ../zlib/')
	aceutils.call(r'make install')
	aceutils.cd(r'../')

	aceutils.cd(r'libpng_bin')
	if aceutils.isMac():
		aceutils.copy(r'../libpng/scripts/pnglibconf.h.prebuilt',r'../libpng/pnglibconf.h')
		aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release -D PNG_STATIC:BOOL=ON -D CMAKE_INSTALL_PREFIX:PATH=../../Dev "-DCMAKE_OSX_ARCHITECTURES=x86_64' + (';i386' if aceutils.Isi386() else '') + r'" ../libpng/')
	else:
		aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX:PATH=../../Dev ../libpng/')
	aceutils.call(r'make install')
	aceutils.cd(r'../../')

	aceutils.rm(r'Dev/lib/libz.so')
	aceutils.rm(r'Dev/lib/libz.so.1')
	aceutils.rm(r'Dev/lib/libz.so.1.2.8')

	aceutils.rm(r'Dev/lib/libpng.so')
	aceutils.rm(r'Dev/lib/libpng16.so')
	aceutils.rm(r'Dev/lib/libpng16.so.16')
	aceutils.rm(r'Dev/lib/libpng16.so.16.6.0')

	aceutils.rm(r'Dev/lib/libz.dylib')
	aceutils.rm(r'Dev/lib/libz.1.dylib')
	aceutils.rm(r'Dev/lib/libz.1.2.8.dylib')

	aceutils.rm(r'Dev/lib/libpng.dylib')
	aceutils.rm(r'Dev/lib/libpng16.dylib')
	aceutils.rm(r'Dev/lib/libpng16.16.dylib')
	aceutils.rm(r'Dev/lib/libpng16.16.6.0.dylib')
