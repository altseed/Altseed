
import aceutils
import datetime
import makeDocumentHtml

def getTargetDir(type):
	d = datetime.datetime.today()
	return 'ACE_' + type.upper() + '_' + str(d.year).zfill(4) + str(d.month).zfill(2) + str(d.day).zfill(2)
	

def init(type, targetDir):
	aceutils.cdToScript()
	aceutils.cd(r'../')
	aceutils.rmdir(targetDir)
	aceutils.rmdir(r'Sample/sample_cs/obj')

def compile(type):
	if aceutils.isWin():
		aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Dev/unitTest_Engine_cpp.sln /p:configuration=Debug')
		aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Dev/unitTest_Engine_cpp.sln /p:configuration=Release')
		aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Dev/unitTest_Engine_cs.sln /p:configuration=Release')

		aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Dev/FontGeneratorWPF.sln /p:configuration=Release')

def copyTool(type, targetDir):
	toolDir=targetDir+r'/Tool/'
	aceutils.mkdir(toolDir)
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

def makeDocument(type, targetDir):
	makeDocumentHtml.make_document_html()
	aceutils.copytree('DocumentHtml',targetDir + '/Document', True)
	aceutils.rmdir('DocumentHtml')


def release_cpp():
	type = 'cpp'
	
	targetDir = getTargetDir(type)
		
	init(type, targetDir)
	
	compile(type)

	aceutils.mkdir(targetDir+r'/')

	copyTool(type, targetDir)

	makeDocument(type, targetDir)

	# Sample
	sampleDir = targetDir+r'/Sample/'
	sampleBinDir = sampleDir+r'bin/'

	aceutils.mkdir(sampleDir)
	aceutils.mkdir(sampleBinDir)

	aceutils.copytreeWithExt(r'Sample/bin/',sampleBinDir,[ r'.h', r'.cpp', r'.filters', r'.vcxproj', r'.cs', r'.csproj', r'.sln', r'.wav', r'.ogg', r'.png', r'.aip'])

	aceutils.mkdir(sampleDir+r'cpp/')
	aceutils.mkdir(sampleDir+r'cpp/include/')
	aceutils.mkdir(sampleDir+r'cpp/lib/')
	aceutils.mkdir(sampleDir+r'cpp/lib/Debug/')
	aceutils.mkdir(sampleDir+r'cpp/lib/Release/')

	aceutils.copy(r'Dev/bin/ace_core.Debug.dll', sampleBinDir)
	aceutils.copy(r'Dev/bin/ace_core.dll', sampleBinDir)

	aceutils.copy(r'Dev/include/ace.h', sampleDir+r'cpp/include/')
	aceutils.copy(r'Dev/lib/Debug/x86/ace_engine.lib', sampleDir+r'cpp/lib/Debug/')
	aceutils.copy(r'Dev/lib/Release/x86/ace_engine.lib', sampleDir+r'cpp/lib/Release/')

	aceutils.copy(r'Sample/sample_cpp.sln', sampleDir)
	aceutils.mkdir(sampleDir+r'sample_cpp/')
	aceutils.copytreeWithExt(r'Sample/sample_cpp/',sampleDir+r'sample_cpp/',[ r'.h', r'.cpp', r'.filters', r'.vcxproj', r'.cs', r'.csproj', r'.sln', r'.wav', r'.ogg', r'.png', r'.aip'])

	# Runtime
	runtimeDir = targetDir+r'/Runtime/'

	aceutils.mkdir(runtimeDir)
	aceutils.mkdir(runtimeDir+r'Debug/')
	aceutils.mkdir(runtimeDir+r'Release/')

	aceutils.copy(r'Dev/bin/ace_core.Debug.dll', runtimeDir)
	aceutils.copy(r'Dev/bin/ace_core.dll', runtimeDir)
	aceutils.copy(r'Dev/include/ace.h', runtimeDir)
	aceutils.copy(r'Dev/lib/x86/Debug/ace_engine.lib',runtimeDir+r'Debug/')
	aceutils.copy(r'Dev/lib/x86/Release/ace_engine.lib', runtimeDir+r'Release/')

	# Doxygen
	aceutils.call(r'doxygen Script/acecppDoxyfile')
	aceutils.copytree(r'Reference_CPP/html/', targetDir+r'/Reference_CPP')




def release_cs():
	type = 'cs'
	
	targetDir = getTargetDir(type)
		
	init(type, targetDir)
	
	compile(type)

	aceutils.mkdir(targetDir+r'/')

	copyTool(type, targetDir)

	makeDocument(type, targetDir)

	# Sample
	sampleDir = targetDir+r'/Sample/'
	sampleBinDir = sampleDir+r'bin/'

	aceutils.mkdir(sampleDir)
	aceutils.mkdir(sampleBinDir)
	
	aceutils.copytreeWithExt(r'Sample/bin/',sampleBinDir,[ r'.h', r'.cpp', r'.filters', r'.vcxproj', r'.cs', r'.csproj', r'.sln', r'.wav', r'.ogg', r'.png', r'.aip'])

	aceutils.copy(r'Dev/bin/ace_core.dll', sampleBinDir)
	aceutils.copy(r'Dev/bin/ace_cs.dll', sampleBinDir)

	aceutils.copy(r'Sample/sample_cs.sln', sampleDir)
	aceutils.mkdir(sampleDir+r'sample_cs/')
	aceutils.copytreeWithExt(r'Sample/sample_cs/',sampleDir+r'sample_cs/',[ r'.h', r'.cpp', r'.filters', r'.vcxproj', r'.cs', r'.csproj', r'.sln', r'.wav', r'.ogg', r'.png', r'.aip'])
	
	aceutils.copy(r'Dev/bin/ace_cs.dll', sampleDir+r'sample_cs/')
	aceutils.copy(r'Dev/bin/ace_cs.XML', sampleDir+r'sample_cs/')

	# Runtime
	runtimeDir = targetDir+r'/Runtime/'

	aceutils.mkdir(runtimeDir)

	aceutils.copy(r'Dev/bin/ace_core.dll', runtimeDir)
	aceutils.copy(r'Dev/bin/ace_cs.dll', runtimeDir)
	aceutils.copy(r'Dev/bin/ace_cs.XML', runtimeDir)




release_cpp()
release_cs()






