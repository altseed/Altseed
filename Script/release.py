
import aceutils
import datetime
import makeDocumentHtml
import os
import os.path

def getTargetDir(type):
	d = datetime.datetime.today()
	common = 'Altseed_' + type.upper() + '_' + str(d.year).zfill(4) + str(d.month).zfill(2) + str(d.day).zfill(2)

	if aceutils.isWin():
		return common + '_WIN'

	if aceutils.isMac():
		return common + '_MAC'
	return common	


def init(type, targetDir):
	aceutils.cdToScript()
	aceutils.cd(r'../')
	aceutils.rmdir(targetDir)
	aceutils.rmdir(r'Sample/BasicSample/sample_cs/obj')

def compile(type):
	if aceutils.isWin():
		aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Dev/unitTest_Engine_cpp.sln /p:configuration=Debug')
		aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Dev/unitTest_Engine_cpp.sln /p:configuration=Release')
		aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Dev/unitTest_Engine_cs.sln /p:configuration=Release')

		aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Dev/FontGeneratorWPF.sln /p:configuration=Release')

		aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Dev/FilePackageGenerator.sln /p:configuration=Release')

	elif aceutils.isMac():
		if type=='cpp':
			aceutils.rmdir(r'Dev/cmake')
			aceutils.mkdir(r'Dev/cmake')
			aceutils.cd(r'Dev/cmake')
			aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX:PATH=../ "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../')
			aceutils.call(r'make install')
			aceutils.cd(r'../../')

		elif type=='cs':
			aceutils.cd(r'Dev')
			aceutils.call(r'xbuild /p:Configuration=Release unitTest_Engine_cs.sln')
			aceutils.cd(r'../')

def copyTool(type, targetDir):
	toolDir=targetDir+r'/Tool/'
	aceutils.mkdir(toolDir)
	if aceutils.isWin():
		aceutils.copy(r'Dev/bin/FontGenerator.WPF.exe', toolDir)
		aceutils.copy(r'Dev/bin/FontGenerator.WPF.exe.config', toolDir)
		aceutils.copy(r'Dev/bin/FontGenerator.Model.dll', toolDir)
		aceutils.copy(r'Dev/bin/FontGeneratorCore.dll', toolDir)
		aceutils.copy(r'Dev/bin/nkf32.dll', toolDir)
		aceutils.copy(r'Dev/bin/System.Reactive.Core.dll', toolDir)
		aceutils.copy(r'Dev/bin/System.Reactive.Interfaces.dll', toolDir)
		aceutils.copy(r'Dev/bin/System.Reactive.Linq.dll', toolDir)
		aceutils.copy(r'Dev/bin/System.Reactive.PlatformServices.dll', toolDir)
		
		aceutils.copy(r'Dev/bin/ImagePackageGenerator.exe', toolDir)
		aceutils.copy(r'Dev/bin/ImagePackageGenerator.exe.config', toolDir)
		aceutils.copy(r'Dev/bin/PSDParser.dll', toolDir)

		aceutils.copy(r'Dev/bin/FilePackageGenerator.exe', toolDir)
		aceutils.copy(r'Dev/bin/FilePackageGenerator.GUI.exe', toolDir)
		aceutils.copy(r'Dev/bin/FilePackageGenerator.GUI.exe.config', toolDir)
		aceutils.copy(r'Dev/bin/FilePackageGeneratorCore.dll', toolDir)

def makeDocument(type, targetDir,mode):
	makeDocumentHtml.make_document_html(mode)
	aceutils.copytree('DocumentHtml',targetDir + '/Document', True)
	aceutils.rmdir('DocumentHtml')

def editCSFiles(targetDir):
	files = []
	for f in aceutils.get_files(targetDir):
		basename = os.path.basename(f)
		ext = os.path.splitext(basename)[1]
		if ext == '.cs':
			files.append(f)

	for file in files:
		ls = []
		with open(file, mode='r', encoding='utf-8-sig') as f:
			for l in f.readlines():
				if 'Recorder.TakeScreenShot' in l:
					continue
				if 'Recorder.CaptureScreen' in l:
					continue

				else:
					ls.append(l)

		with open(file, mode='w',  encoding='utf-8') as f:
			f.writelines(ls)

def release_cpp():
	type = 'cpp'
	
	targetDir = getTargetDir(type)
		
	init(type, targetDir)
	
	compile(type)

	aceutils.mkdir(targetDir+r'/')

	copyTool(type, targetDir)

	makeDocument(type, targetDir,'cpp')

	# Sample
	aceutils.mkdir(targetDir+r'/Sample/')

	def copySampleFiles(from_,to_):
		sampleDir = to_
		sampleBinDir = sampleDir+r'bin/'
		aceutils.mkdir(sampleDir)
		aceutils.mkdir(sampleBinDir)
		aceutils.copytreeWithExt(from_ + 'bin/',sampleBinDir,[ r'.h', r'.cpp', r'.filters', r'.config', r'.vcxproj', r'.cs', r'.csproj', r'.sln', r'.wav', r'.ogg', r'.png', r'.aip', r'.efk', r'.aff', r'.pack', r'.txt'])

		aceutils.mkdir(sampleDir+r'cpp/')
		aceutils.mkdir(sampleDir+r'cpp/include/')
		aceutils.mkdir(sampleDir+r'cpp/lib/')
		aceutils.mkdir(sampleDir+r'cpp/lib/Debug/')
		aceutils.mkdir(sampleDir+r'cpp/lib/Release/')

		if aceutils.isWin():
			aceutils.copy(r'Dev/bin/Altseed_core.Debug.dll', sampleBinDir)
			aceutils.copy(r'Dev/bin/Altseed_core.dll', sampleBinDir)
		elif aceutils.isMac():
			aceutils.copy(r'Dev/bin/libAltseed_core.dylib', sampleBinDir)

		aceutils.copy(r'Dev/include/Altseed.h', sampleDir+r'cpp/include/')
		if aceutils.isWin():
			aceutils.copy(r'Dev/lib/x86/Debug/Altseed.lib', sampleDir+r'cpp/lib/Debug/')
			aceutils.copy(r'Dev/lib/x86/Release/Altseed.lib', sampleDir+r'cpp/lib/Release/')
		elif aceutils.isMac():
			aceutils.copy(r'Dev/lib/libAltseed.a', sampleDir+r'cpp/lib/')

		aceutils.copy(r'Sample/BasicSample/sample_cpp.sln', sampleDir)
		aceutils.mkdir(sampleDir+r'sample_cpp/')
		aceutils.copytreeWithExt(from_ + r'sample_cpp/',sampleDir+r'sample_cpp/',[ r'.h', r'.cpp', r'.filters', r'.config', r'.vcxproj', r'.cs', r'.csproj', r'.sln', r'.wav', r'.ogg', r'.png', r'.aip', r'.efk', r'.aff', r'.pack', r'.txt'])

	copySampleFiles(r'Sample/BasicSample/',targetDir+r'/Sample/BasicSample/')
	copySampleFiles(r'Sample/ApplicationSample/',targetDir+r'/Sample/ApplicationSample/')

	# Runtime
	runtimeDir = targetDir+r'/Runtime/'

	aceutils.mkdir(runtimeDir)
	aceutils.mkdir(runtimeDir+r'Debug/')
	aceutils.mkdir(runtimeDir+r'Release/')

	if aceutils.isWin():
		aceutils.copy(r'Dev/bin/Altseed_core.Debug.dll', runtimeDir)
		aceutils.copy(r'Dev/bin/Altseed_core.dll', runtimeDir)
	elif aceutils.isMac():
		aceutils.copy(r'Dev/bin/libAltseed_core.dylib', runtimeDir)
	aceutils.copy(r'Dev/include/Altseed.h', runtimeDir)
	if aceutils.isWin():
		aceutils.copy(r'Dev/lib/x86/Debug/Altseed.lib',runtimeDir+r'Debug/')
		aceutils.copy(r'Dev/lib/x86/Release/Altseed.lib', runtimeDir+r'Release/')
	elif aceutils.isMac():
		aceutils.copy(r'Dev/lib/libAltseed.a',runtimeDir+r'/')

	# Doxygen
	aceutils.call(r'doxygen Script/Doxyfile_cpp')
	aceutils.copytree(r'Reference_CPP/html/', targetDir+r'/Reference_CPP')

	# Readme
	aceutils.copy(r'readme_cpp.txt', targetDir+r'/readme.txt')

	# Template
	if aceutils.isWin():
		aceutils.copytreeWithExt(r'Template/cpp_win/',targetDir+r'/Template/',[ r'.h', r'.cpp', r'.filters', r'.config', r'.vcxproj', r'.sln'])
		aceutils.mkdir(targetDir+r'/Template/bin/')
		aceutils.mkdir(targetDir+r'/Template/cpp/')
		aceutils.mkdir(targetDir+r'/Template/cpp/include/')
		aceutils.mkdir(targetDir+r'/Template/cpp/lib/')
		aceutils.mkdir(targetDir+r'/Template/cpp/lib/Debug/')
		aceutils.mkdir(targetDir+r'/Template/cpp/lib/Release/')

		aceutils.copy(r'Dev/bin/Altseed_core.Debug.dll', targetDir+r'/Template/bin/')
		aceutils.copy(r'Dev/bin/Altseed_core.dll', targetDir+r'/Template/bin/')
		aceutils.copy(r'Dev/include/Altseed.h', targetDir+r'/Template/cpp/include/')
		aceutils.copy(r'Dev/lib/x86/Debug/Altseed.lib', targetDir+r'/Template/cpp/lib/Debug/')
		aceutils.copy(r'Dev/lib/x86/Release/Altseed.lib', targetDir+r'/Template/cpp/lib/Release/')
	elif aceutils.isMac():
		aceutils.copytreeWithExt(r'Template/cpp_mac/',targetDir+r'/Template/',[ r'.h', r'.cpp', r'.pbxproj', r'.xcworkspacedata'])
		aceutils.mkdir(targetDir+r'/Template/bin/')
		aceutils.mkdir(targetDir+r'/Template/include/')
		aceutils.mkdir(targetDir+r'/Template/lib/')
		aceutils.copy(r'Dev/bin/libAltseed_core.dylib', targetDir+r'/Template/bin/')
		aceutils.copy(r'Dev/include/Altseed.h', targetDir+r'/Template/include/')
		aceutils.copy(r'Dev/lib/libAltseed.a', targetDir+r'/Template/lib/')

def release_cs():
	type = 'cs'
	
	targetDir = getTargetDir(type)
		
	init(type, targetDir)
	
	compile(type)

	aceutils.mkdir(targetDir+r'/')

	copyTool(type, targetDir)

	makeDocument(type, targetDir,'cs')

	# Sample
	aceutils.mkdir(targetDir+r'/Sample/')

	def copySampleFiles(from_,to_):
		sampleDir = to_
		sampleBinDir = sampleDir+r'bin/'

		aceutils.mkdir(sampleDir)
		aceutils.mkdir(sampleBinDir)
	
		aceutils.copytreeWithExt(from_ + r'bin/',sampleBinDir,[ r'.h', r'.cpp', r'.filters', r'.config', r'.vcxproj', r'.cs', r'.csproj', r'.sln', r'.wav', r'.ogg', r'.png', r'.aip', r'.efk', r'.aff', r'.pack', r'.txt'])

		if aceutils.isWin():
			aceutils.copy(r'Dev/bin/Altseed_core.dll', sampleBinDir)
		elif aceutils.isMac():
			aceutils.copy(r'Dev/bin/libAltseed_core.dylib', sampleBinDir)

		aceutils.copy(r'Sample/sample_cs.sln', sampleDir)
		aceutils.mkdir(sampleDir+r'sample_cs/')
		aceutils.copytreeWithExt(from_ + r'sample_cs/',sampleDir+r'sample_cs/',[ r'.h', r'.cpp', r'.filters', r'.config', r'.vcxproj', r'.cs', r'.csproj', r'.sln', r'.wav', r'.ogg', r'.png', r'.aip', r'.efk', r'.aff', r'.pack', r'.txt'])

		editCSFiles(sampleDir+r'sample_cs/')

		aceutils.copy(r'Dev/bin/Altseed.dll', sampleDir+r'sample_cs/')
		aceutils.copy(r'Dev/bin/Altseed.XML', sampleDir+r'sample_cs/')

	copySampleFiles(r'Sample/BasicSample/',targetDir+r'/Sample/BasicSample/')
	copySampleFiles(r'Sample/ApplicationSample/',targetDir+r'/Sample/ApplicationSample/')

	# Runtime
	runtimeDir = targetDir+r'/Runtime/'

	aceutils.mkdir(runtimeDir)

	if aceutils.isWin():
		aceutils.copy(r'Dev/bin/Altseed_core.dll', runtimeDir)
	elif aceutils.isMac():
		aceutils.copy(r'Dev/bin/libAltseed_core.dylib', runtimeDir)
	aceutils.copy(r'Dev/bin/Altseed.dll', runtimeDir)
	aceutils.copy(r'Dev/bin/Altseed.XML', runtimeDir)

	# Readme
	aceutils.copy(r'readme_cs.txt', targetDir+r'/readme.txt')

	# Template
	aceutils.copytreeWithExt(r'Template/cs_win/',targetDir+r'/Template/',[r'.cs', r'.filters', r'.config', r'.vcxproj', r'.sln', r'.csproj'])
	aceutils.mkdir(targetDir+r'/Template/bin/')
	aceutils.copy(r'Dev/bin/Altseed.dll', targetDir+r'/Template/Game/')
	aceutils.copy(r'Dev/bin/Altseed.XML', targetDir+r'/Template/Game/')
	aceutils.copy(r'Dev/bin/Altseed_core.dll', targetDir+r'/Template/bin/')

release_cpp()
release_cs()






