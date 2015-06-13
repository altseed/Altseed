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

	aceutils.copy(r'Dev/bin/Altseed.dll', sampleDir+r'bin/')
	aceutils.copy(r'Dev/bin/Altseed_core.Debug.dll', sampleDir+r'bin/')
	aceutils.copy(r'Dev/bin/Altseed_core.dll', sampleDir+r'bin/')

	aceutils.copy(r'Dev/include/Altseed.h', sampleDir+r'cpp/include/')
	aceutils.copy(r'Dev/lib/x86/Debug/Altseed.lib', sampleDir+r'cpp/lib/Debug/')
	aceutils.copy(r'Dev/lib/x86/Release/Altseed.lib', sampleDir+r'cpp/lib/Release/')

	aceutils.copy(r'Dev/bin/Altseed.dll', sampleDir+r'sample_cs/')
	aceutils.copy(r'Dev/bin/Altseed.XML', sampleDir+r'sample_cs/')

copyDev(r'Sample')
copyDev(r'Sample/BasicSample')

