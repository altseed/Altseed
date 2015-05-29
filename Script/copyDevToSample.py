import os.path
import aceutils

aceutils.cdToScript()
aceutils.cd(r'../')

if aceutils.isWin():
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Dev/unitTest_Engine_cpp.sln /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Dev/unitTest_Engine_cpp.sln /p:configuration=Release')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Dev/unitTest_Engine_cs.sln /p:configuration=Release')

def copyDev(targetDir):
	aceutils.mkdir(targetDir+r'/')

	# Sample
	sampleDir = targetDir+r'/'

	aceutils.mkdir(sampleDir+r'cpp/')
	aceutils.mkdir(sampleDir+r'cpp/include/')
	aceutils.mkdir(sampleDir+r'cpp/lib/')
	aceutils.mkdir(sampleDir+r'cpp/lib/Debug/')
	aceutils.mkdir(sampleDir+r'cpp/lib/Release/')

	aceutils.copy(r'Dev/bin/ace_cs.dll', sampleDir+r'bin/')
	aceutils.copy(r'Dev/bin/ace_core.Debug.dll', sampleDir+r'bin/')
	aceutils.copy(r'Dev/bin/ace_core.dll', sampleDir+r'bin/')

	aceutils.copy(r'Dev/include/ace.h', sampleDir+r'cpp/include/')
	aceutils.copy(r'Dev/lib/x86/Debug/ace_engine.lib', sampleDir+r'cpp/lib/Debug/')
	aceutils.copy(r'Dev/lib/x86/Release/ace_engine.lib', sampleDir+r'cpp/lib/Release/')

	aceutils.copy(r'Dev/bin/ace_cs.dll', sampleDir+r'sample_cs/')
	aceutils.copy(r'Dev/bin/ace_cs.XML', sampleDir+r'sample_cs/')

copyDev(r'Sample')
copyDev(r'Sample/BasicSample')

