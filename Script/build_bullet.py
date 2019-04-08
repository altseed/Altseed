
import aceutils

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):
	aceutils.rmdir(r"bullet_bin")
	aceutils.rmdir(r"bullet_bin_x64")

	aceutils.editCmakeForACE(r'bullet3/CMakeLists.txt','cp932')
	aceutils.mkdir(r"bullet_bin")
	aceutils.mkdir(r"bullet_bin_x64")

	options = []
	options += ['-D', 'BUILD_PYBULLET=OFF']
	options += ['-D', 'BUILD_EXTRAS=OFF']
	options += ['-D', 'BUILD_UNIT_TESTS=OFF']
	options += ['-D', 'BUILD_CPU_DEMOS=OFF']
	options += ['-D', 'BUILD_BULLET2_DEMOS=OFF']
	options += ['-D', 'BUILD_OPENGL3_DEMOS=OFF']
	options += ['-D', 'USE_GRAPHICAL_BENCHMARK=OFF']
	options_cmd = ' '.join(options)

	with aceutils.CurrentDir('bullet_bin'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ' + options_cmd + r' ../bullet3/')
			aceutils.call('cmake --build . --config Debug')
			aceutils.call('cmake --build . --config Release')
		elif aceutils.isMac():
			aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release ' + options_cmd + r'  "-DCMAKE_OSX_ARCHITECTURES=x86_64' + (';i386' if aceutils.Isi386() else '') + r'" ../bullet3/')
			aceutils.call(r'make')
		else:
			aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release ' + options_cmd + r' ../bullet3/')
			aceutils.call(r'make')

	with aceutils.CurrentDir('bullet_bin_x64'):
		if aceutils.isWin():
			aceutils.call(aceutils.cmd_cmake_x64+r'-D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ' + options_cmd + r' ../bullet3/')
			aceutils.call('cmake --build . --config Debug')
			aceutils.call('cmake --build . --config Release')

	aceutils.copytreeWithExt(r'bullet3/src/',r'../Dev/include/',['.h'])

	if aceutils.isWin():
		aceutils.mkdir(r'../Dev/lib/x86/')
		aceutils.mkdir(r'../Dev/lib/x86/Debug')
		aceutils.mkdir(r'../Dev/lib/x86/Release')

		aceutils.mkdir(r'../Dev/lib/x64/')
		aceutils.mkdir(r'../Dev/lib/x64/Debug')
		aceutils.mkdir(r'../Dev/lib/x64/Release')

		aceutils.copy(r'bullet_bin/lib/Debug/BulletCollision_Debug.lib', r'../Dev/lib/x86/Debug/')
		aceutils.copy(r'bullet_bin/lib/Debug/LinearMath_Debug.lib', r'../Dev/lib/x86/Debug/')

		aceutils.copy(r'bullet_bin/lib/Release/BulletCollision.lib', r'../Dev/lib/x86/Release/')
		aceutils.copy(r'bullet_bin/lib/Release/LinearMath.lib', r'../Dev/lib/x86/Release/')

		aceutils.copy(r'bullet_bin_x64/lib/Debug/BulletCollision_Debug.lib', r'../Dev/lib/x64/Debug/')
		aceutils.copy(r'bullet_bin_x64/lib/Debug/LinearMath_Debug.lib', r'../Dev/lib/x64/Debug/')

		aceutils.copy(r'bullet_bin_x64/lib/Release/BulletCollision.lib', r'../Dev/lib/x64/Release/')
		aceutils.copy(r'bullet_bin_x64/lib/Release/LinearMath.lib', r'../Dev/lib/x64/Release/')

	else:
		aceutils.copy(r'bullet_bin/src/BulletCollision/libBulletCollision.a', r'../Dev/lib/')
		aceutils.copy(r'bullet_bin/src/LinearMath/libLinearMath.a', r'../Dev/lib/')
