
import aceutils
import datetime
import makeDocumentHtml
import os
import os.path
import sys

versionNumber = '116'

leastCompileTarget = ''

def getTargetDir(type):
	common = 'Altseed_' + type.upper() + '_' + versionNumber
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

def compile_tool():
	if(leastCompileTarget != 'cs'):
		return

	if aceutils.isWin():
		aceutils.call(aceutils.cmd_compile + r'Dev/FontGenerator.sln /p:configuration=Release;platform=x86')
		aceutils.call(aceutils.cmd_compile + r'Dev/FilePackageGenerator.sln /p:configuration=Release;platform=x86')
		aceutils.call(aceutils.cmd_compile + r'Dev/ImagePackageGenerator.sln /p:configuration=Release;platform=x86')

	elif aceutils.isMac():

		# for core
		aceutils.cd(r'Dev/cmake')
		aceutils.call(r'make install')
		aceutils.cd(r'../../')

		aceutils.call(r'nuget restore Dev/FontGenerator.sln -PackagesDirectory Dev/FontGenerator/packages/')
		aceutils.call(r'msbuild Dev/FontGenerator/FontGenerator.Altseed/FontGenerator.Altseed.csproj /p:Configuration=Release /p:Platform=x86')
		aceutils.call(r'msbuild Dev/FilePackageGenerator/FilePackageGenerator.Altseed/FilePackageGenerator.Altseed.csproj /p:Configuration=Release /p:Platform=x86')
		aceutils.call(r'msbuild Dev/ImagePackageGenerator/ImagePackageGenerator/ImagePackageGenerator.csproj /p:Configuration=Release /p:Platform=x86')

def compile(type):
	global leastCompileTarget
	leastCompileTarget = type
	if aceutils.isWin():
		if type=='cpp':
			aceutils.call(aceutils.cmd_compile + r'Dev/unitTest_Engine_cpp.sln /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'Dev/unitTest_Engine_cpp.sln /p:configuration=Release')
		
		if type=='cs':
			aceutils.call(aceutils.cmd_compile + r'Dev/unitTest_Engine_cs.sln /p:configuration=Release')

		if type=='java':
			aceutils.call(aceutils.cmd_compile + r'Dev/unitTest_Engine_cpp.sln /p:configuration=Release')
			aceutils.cd(r'Dev/asd_java')
			aceutils.call(r'ant')
			aceutils.cd(r'../../')

	elif aceutils.isMac():
		if type=='cpp':
			aceutils.cd(r'Dev/cmake')
			aceutils.call(r'make install')
			aceutils.cd(r'../../')

		elif type=='cs':
			# for core
			aceutils.cd(r'Dev/cmake')
			aceutils.call(r'make install')
			aceutils.cd(r'../../')

			aceutils.cd(r'Dev')
			aceutils.call(r'xbuild /p:Configuration=Release unitTest_Engine_cs.sln')
			aceutils.cd(r'../')

		elif type=='java':
			# for core
			aceutils.cd(r'Dev/cmake')
			aceutils.call(r'make install')
			aceutils.cd(r'../../')

			aceutils.cd(r'Dev/asd_java')
			aceutils.call(r'ant')
			aceutils.cd(r'../../')

def copy_tools(type, targetDir):
	toolDir=targetDir+r'/Tool'
	aceutils.mkdir(toolDir)
	aceutils.copytree(r'Altseed_Tool', toolDir, True)

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


def release_common():
	aceutils.cdToScript()
	aceutils.cd(r'../')

	# generate makefiles using cmake

	if aceutils.isMac():
		aceutils.rmdir(r'Dev/cmake')
		aceutils.mkdir(r'Dev/cmake')
		aceutils.cd(r'Dev/cmake')
		aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release -D BUILD_TOOL=ON -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX:PATH=../ "-DCMAKE_OSX_ARCHITECTURES=x86_64" ../')
		aceutils.cd(r'../../')

def store_tools():
	aceutils.cdToScript()
	aceutils.cd(r'../')
	aceutils.call(sys.executable + r' Dev/generate_swig.py')

	compile('cs')
	compile_tool()

	toolDir = 'Altseed_Tool'
	aceutils.rmdir(toolDir)
	aceutils.mkdir(toolDir)

	aceutils.copy(r'Dev/bin/System.Reactive.dll', toolDir)
	#aceutils.copy(r'Dev/bin/System.Windows.Interactivity.dll', toolDir)
	aceutils.copy(r'Dev/bin/ReactiveProperty.dll', toolDir)
	aceutils.copy(r'Dev/bin/ReactiveProperty.NET46.dll', toolDir)

	aceutils.copy(r'Dev/bin/FontGenerator.exe', toolDir)
	aceutils.copy(r'Dev/bin/FontGenerator.exe.config', toolDir)
	aceutils.copy(r'Dev/bin/FontGenerator.Model.dll', toolDir)
		
	aceutils.copy(r'Dev/bin/ImagePackageGenerator.exe', toolDir)
	aceutils.copy(r'Dev/bin/ImagePackageGenerator.exe.config', toolDir)

	aceutils.copy(r'Dev/bin/FilePackageGenerator.exe', toolDir)
	#aceutils.copy(r'Dev/bin/FilePackageGenerator.exe.config', toolDir)
	aceutils.copy(r'Dev/bin/FilePackageGeneratorCore.dll', toolDir)

	aceutils.copy(r'Dev/bin/Altseed.dll', toolDir)

	if aceutils.isWin():
		aceutils.copy(r'Dev/bin/Altseed_core.dll', toolDir)
		aceutils.copy(r'Dev/bin/FontGeneratorCore.dll', toolDir)
		aceutils.copy(r'Dev/bin/PSDParser.dll', toolDir)
	elif aceutils.isMac():
		aceutils.copy(r'Dev/bin/libAltseed_core.dylib', toolDir)
		aceutils.copy(r'Dev/bin/libFontGeneratorCore.dylib', toolDir)
		aceutils.copy(r'Dev/cmake/ImagePackageGenerator/bin/libPSDParser.dylib', toolDir)


def release_cpp():
	type = 'cpp'
	
	targetDir = getTargetDir(type)
		
	init(type, targetDir)
	
	compile(type)

	aceutils.mkdir(targetDir+r'/')

	copy_tools(type, targetDir)

	makeDocument(type, targetDir,'cpp')

	# Sample
	aceutils.mkdir(targetDir+r'/Sample/')

	def copySampleFiles(from_,to_):
		sampleDir = to_
		sampleBinDir = sampleDir+r'bin/'
		aceutils.mkdir(sampleDir)
		aceutils.mkdir(sampleBinDir)
		aceutils.copytreeWithExt(from_ + 'bin/',sampleBinDir,[ r'.h', r'.cpp', r'.filters', r'.config', r'.vcxproj', r'.cs', r'.csproj', r'.sln', r'.wav', r'.ogg', r'.png', r'.mp4', r'.aip', r'.efk', r'.aff', r'.pack', r'.txt'])

		aceutils.mkdir(sampleDir+r'cpp/')
		aceutils.mkdir(sampleDir+r'cpp/include/')
		aceutils.mkdir(sampleDir+r'cpp/lib/')
		aceutils.mkdir(sampleDir+r'cpp/lib/Debug/')
		aceutils.mkdir(sampleDir+r'cpp/lib/Release/')

		if aceutils.isWin():
			aceutils.copy(r'Dev/bin/Altseed_core_Debug.dll', sampleBinDir)
			aceutils.copy(r'Dev/bin/Altseed_core.dll', sampleBinDir)
		elif aceutils.isMac():
			aceutils.copy(r'Dev/bin/libAltseed_core.dylib', sampleBinDir)

		aceutils.copy(r'Dev/include/Altseed.h', sampleDir+r'cpp/include/')
		if aceutils.isWin():
			aceutils.copy(r'Dev/lib/x86/Debug/Altseed.lib', sampleDir+r'cpp/lib/Debug/')
			aceutils.copy(r'Dev/lib/x86/Release/Altseed.lib', sampleDir+r'cpp/lib/Release/')
		elif aceutils.isMac():
			aceutils.copy(r'Dev/lib/libAltseed.a', sampleDir+r'cpp/lib/')

		aceutils.copy(from_+r'sample_cpp.sln',to_)

		aceutils.mkdir(sampleDir+r'sample_cpp/')
		aceutils.copytreeWithExt(from_ + r'sample_cpp/',sampleDir+r'sample_cpp/',[ r'.h', r'.cpp', r'.filters', r'.config', r'.vcxproj', r'.cs', r'.csproj', r'.sln', r'.wav', r'.ogg', r'.png', r'.mp4', r'.aip', r'.efk', r'.aff', r'.pack', r'.txt'])

	copySampleFiles(r'Sample/BasicSample/',targetDir+r'/Sample/BasicSample/')
	copySampleFiles(r'Sample/ApplicationSample/',targetDir+r'/Sample/ApplicationSample/')

	# Runtime
	runtimeDir = targetDir+r'/Runtime/'

	aceutils.mkdir(runtimeDir)
	aceutils.mkdir(runtimeDir+r'Debug/')
	aceutils.mkdir(runtimeDir+r'Release/')

	if aceutils.isWin():
		aceutils.copy(r'Dev/bin/Altseed_core_Debug.dll', runtimeDir)
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
	aceutils.copytree(r'Doxygen_CPP/html/', targetDir+r'/Reference_CPP')

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

		aceutils.copy(r'Dev/bin/Altseed_core_Debug.dll', targetDir+r'/Template/bin/')
		aceutils.copy(r'Dev/bin/Altseed_core.dll', targetDir+r'/Template/bin/')
		aceutils.copy(r'Dev/include/Altseed.h', targetDir+r'/Template/cpp/include/')
		aceutils.copy(r'Dev/lib/x86/Debug/Altseed.lib', targetDir+r'/Template/cpp/lib/Debug/')
		aceutils.copy(r'Dev/lib/x86/Release/Altseed.lib', targetDir+r'/Template/cpp/lib/Release/')
	elif aceutils.isMac():
		aceutils.copytreeWithExt(r'Template/cpp_mac/',targetDir+r'/Template/',[ r'.h', r'.cpp', r'.plist', r'.pbxproj', r'.xcworkspacedata'])
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
	
	# GenerateHeader
	aceutils.call(sys.executable + r' Dev/generate_swig.py')
	
	compile(type)

	aceutils.mkdir(targetDir+r'/')

	copy_tools(type, targetDir)

	makeDocument(type, targetDir,'cs')

	# Sample
	aceutils.mkdir(targetDir+r'/Sample/')

	def copySampleFiles(from_,to_):
		sampleDir = to_
		sampleBinDir = sampleDir+r'bin/'

		aceutils.mkdir(sampleDir)
		aceutils.mkdir(sampleBinDir)
	
		aceutils.copytreeWithExt(from_ + r'bin/',sampleBinDir,[ r'.h', r'.cpp', r'.filters', r'.config', r'.vcxproj', r'.cs', r'.csproj', r'.sln', r'.wav', r'.ogg', r'.png', r'.mp4', r'.aip', r'.efk', r'.aff', r'.pack', r'.txt'])

		if aceutils.isWin():
			aceutils.copy(r'Dev/bin/Altseed_core.dll', sampleBinDir)
		elif aceutils.isMac():
			aceutils.copy(r'Dev/bin/libAltseed_core.dylib', sampleBinDir)

		aceutils.mkdir(sampleDir+r'sample_cs/')
		aceutils.copytreeWithExt(from_ + r'sample_cs/',sampleDir+r'sample_cs/',[ r'.h', r'.cpp', r'.filters', r'.config', r'.vcxproj', r'.cs', r'.csproj', r'.sln', r'.wav', r'.ogg', r'.png', r'.mp4', r'.aip', r'.efk', r'.aff', r'.pack', r'.txt'])

		editCSFiles(sampleDir+r'sample_cs/')
		
		aceutils.copy(from_+r'sample_cs.sln',to_)

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
	if aceutils.isWin():
		aceutils.copytreeWithExt(r'Template/cs_win/',targetDir+r'/Template/',[r'.cs', r'.filters', r'.config', r'.vcxproj', r'.sln', r'.csproj'])
		aceutils.mkdir(targetDir+r'/Template/bin/')
		aceutils.copy(r'Dev/bin/Altseed.dll', targetDir+r'/Template/Game/')
		aceutils.copy(r'Dev/bin/Altseed.XML', targetDir+r'/Template/Game/')
		aceutils.copy(r'Dev/bin/Altseed_core.dll', targetDir+r'/Template/bin/')
	elif aceutils.isMac():
		aceutils.copytreeWithExt(r'Template/cs_mac/',targetDir+r'/Template/',[r'.cs', r'.plist', r'.sln', r'.csproj'])
		aceutils.mkdir(targetDir+r'/Template/bin/')
		aceutils.copy(r'Dev/bin/Altseed.dll', targetDir+r'/Template/Game/')
		aceutils.copy(r'Dev/bin/Altseed.XML', targetDir+r'/Template/Game/')
		aceutils.copy(r'Dev/bin/libAltseed_core.dylib', targetDir+r'/Template/bin/')

def release_java():
	type = 'java'

	targetDir = getTargetDir(type)
		
	init(type, targetDir)

	# GenerateHeader	
	aceutils.call(sys.executable + r' Dev/generate_swig.py java')
	aceutils.call(sys.executable + r' Script/generateTranslatedCode.py java')
		
	# Sample
	aceutils.call(sys.executable + r' Script/generate_sample.py java')

	aceutils.cd(r'Sample/BasicSample/sample_java/')
	aceutils.call(r'ant')
	aceutils.cd(r'../../../')

	aceutils.cd(r'Sample/ApplicationSample/sample_java/')
	aceutils.call(r'ant')
	aceutils.cd(r'../../../')


	compile(type)

	aceutils.mkdir(targetDir+r'/')

	copy_tools(type, targetDir)

	makeDocument(type, targetDir,'java')

	# Sample
	aceutils.mkdir(targetDir+r'/Sample/')

	def copySampleFiles(from_,to_):

		sampleDir = to_
		sampleBinDir = sampleDir+r'bin/'

		aceutils.mkdir(sampleDir)
		aceutils.mkdir(sampleBinDir)
	
		aceutils.copytreeWithExt(from_ + r'bin/',sampleBinDir,[ r'.jar', r'.wav', r'.ogg', r'.png', r'.mp4', r'.aip', r'.efk', r'.aff', r'.pack', r'.txt'])

		if aceutils.isWin():
			aceutils.copy(r'Dev/bin/Altseed_core.dll', sampleBinDir)
		elif aceutils.isMac():
			aceutils.copy(r'Dev/bin/libAltseed_core.dylib', sampleBinDir)

		aceutils.mkdir(sampleDir+r'sample_java/')
		aceutils.copytreeWithExt(from_ + r'sample_java/',sampleDir+r'sample_java/',[ r'.java', r'.xml'])

		aceutils.copy(r'Dev/bin/Altseed.jar', sampleBinDir)

	def editFiles(targetDir):
		files = []
		for f in aceutils.get_files(targetDir):
			basename = os.path.basename(f)
			ext = os.path.splitext(basename)[1]
			
			if ext == '.java':
				if r'Sample' in basename or 'Program' in basename:
					aceutils.rm(f)
				else:
					files.append(f)

		for file in files:
			ls = []
			with open(file, mode='r', encoding='utf-8-sig') as f:
				for l in f.readlines():
					l = l.replace(r'public void Run()','public static void main(String args[])')
					l = l.replace(r'implements ISample','')
					ls.append(l)
	
			with open(file, mode='w', encoding='utf-8-sig') as f:
				f.writelines(ls)

	copySampleFiles(r'Sample/BasicSample/',targetDir+r'/Sample/BasicSample/')
	copySampleFiles(r'Sample/ApplicationSample/',targetDir+r'/Sample/ApplicationSample/')

	editFiles(targetDir+r'/Sample/BasicSample/')
	editFiles(targetDir+r'/Sample/ApplicationSample/')

	# Runtime
	runtimeDir = targetDir+r'/Runtime/'

	aceutils.mkdir(runtimeDir)

	if aceutils.isWin():
		aceutils.copy(r'Dev/bin/Altseed_core.dll', runtimeDir)
	elif aceutils.isMac():
		aceutils.copy(r'Dev/bin/libAltseed_core.dylib', runtimeDir)
	aceutils.copy(r'Dev/bin/Altseed.jar', runtimeDir)

	# Readme
	aceutils.copy(r'readme_java.txt', targetDir+r'/readme.txt')

	# Template


release_common()

store_tools()

release_cpp()
release_cs()
release_java()



