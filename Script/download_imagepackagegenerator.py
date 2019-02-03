
import aceutils

aceutils.cdToScript()
aceutils.mkdir('../Downloads')

with aceutils.CurrentDir('../Downloads'):
	aceutils.rm(r'master.zip')
	aceutils.rmdir(r'ImagePackageGenerator-master')

	aceutils.wget(r'https://github.com/altseed/ImagePackageGenerator/archive/master.zip')
	aceutils.unzip(r'master.zip')

	with aceutils.CurrentDir('ImagePackageGenerator-master'):
		if aceutils.isWin():
			aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" ImagePackageGenerator.sln /p:configuration=Release')
		elif aceutils.isMac():
			aceutils.call('cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_OSX_ARCHITECTURES="x86_64' + (';i386' if aceutils.Isi386() else '') + ' ./')
			aceutils.call('make')
			aceutils.call(r'xbuild /p:Configuration=Release ImagePackageGenerator.sln')

	aceutils.mkdir(r'../Dev/bin/')
	aceutils.copy(r'ImagePackageGenerator-master/bin/ImagePackageGenerator.exe', r'../Dev/bin/')
	aceutils.copy(r'ImagePackageGenerator-master/bin/ImagePackageGenerator.exe.config', r'../Dev/bin/')
	if aceutils.isWin():
		aceutils.copy(r'ImagePackageGenerator-master/bin/PSDParser.dll', r'../Dev/bin/')
	elif aceutils.isMac():
		aceutils.copy(r'ImagePackageGenerator-master/libPSDParser.dylib', r'../Dev/bin/')
