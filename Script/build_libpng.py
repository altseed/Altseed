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
	
	def compile(xn):
		aceutils.mkdir(r'zlib_bin/{}/'.format(xn))
		aceutils.mkdir(r'libpng_bin/{}/'.format(xn))
	
		aceutils.cd(r'zlib_bin/{}/'.format(xn))
	
		cmake_args = []
		cmake_args += [r'-D CMAKE_INSTALL_PREFIX:PATH=.']
		cmake_args += [r'../../zlib/']
	
		if xn == 'x86':
			aceutils.call(aceutils.cmd_cmake + r' ' + ' '.join(cmake_args))
		else:
			aceutils.call(aceutils.cmd_cmake_x64 + r' ' + ' '.join(cmake_args))
					
		aceutils.call(r'cmake --build . --config Debug --target install')
		aceutils.call(r'cmake --build . --config Release --target install')
		aceutils.cd(r'../../')
	
		aceutils.cd(r'libpng_bin/{}/'.format(xn))

		cmake_args = []
		cmake_args += [r'-D PNG_STATIC=ON']
		cmake_args += [r'-D PNG_SHARED=OFF']
		cmake_args += [r'-D CMAKE_INSTALL_PREFIX:PATH=.']
		cmake_args += [r'-D ZLIB_INCLUDE_DIR=../../zlib_bin/{}/include'.format(xn)]
		cmake_args += [r'-D ZLIB_LIBRARY_DEBUG=../../zlib_bin/{}/lib/zlibstaticd.lib'.format(xn)]
		cmake_args += [r'-D ZLIB_LIBRARY_RELEASE=../../zlib_bin/{}/lib/zlibstatic.lib'.format(xn)]
		cmake_args += [r'../../libpng/']

		if xn == 'x86':
			aceutils.call(aceutils.cmd_cmake + r' ' + ' '.join(cmake_args))
		else:
			aceutils.call(aceutils.cmd_cmake_x64 + r' ' + ' '.join(cmake_args))

		aceutils.call(r'cmake --build . --config Debug --target install')
		aceutils.call(r'cmake --build . --config Release --target install')
		aceutils.cd(r'../../')
	
		aceutils.mkdir(r'../Dev/lib/{}/'.format(xn))
		aceutils.mkdir(r'../Dev/lib/{}/Debug'.format(xn))
		aceutils.mkdir(r'../Dev/lib/{}/Release'.format(xn))

		aceutils.copy(r'zlib_bin/{}/lib/zlibstaticd.lib'.format(xn), r'../Dev/lib/{}/Debug/'.format(xn))
		aceutils.copy(r'zlib_bin/{}/lib/zlibstatic.lib'.format(xn), r'../Dev/lib/{}/Release/'.format(xn))
		aceutils.copy(r'libpng_bin/{}/lib/libpng16_staticd.lib'.format(xn), r'../Dev/lib/{}/Debug/'.format(xn))
		aceutils.copy(r'libpng_bin/{}/lib/libpng16_static.lib'.format(xn), r'../Dev/lib/{}/Release/'.format(xn))

	compile('x86')
	compile('x64')

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
