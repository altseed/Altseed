
import aceutils
import datetime

d = datetime.datetime.today()

targetDir = 'ACE' + str(d.year).zfill(4) + str(d.month).zfill(2) + str(d.day).zfill(2)

aceutils.cdToScript()
aceutils.cd(r'../')


if aceutils.isWin():
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Dev/unitTest_Engine_cpp.sln /p:configuration=Debug')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Dev/unitTest_Engine_cpp.sln /p:configuration=Release')
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" Dev/unitTest_Engine_cs.sln /p:configuration=Release')

#remove files
aceutils.rmdir(targetDir)
aceutils.rmdir(r'Sample/sample_cs/obj')


aceutils.mkdir(targetDir+r'/')

# Sample
sampleDir = targetDir+r'/Sample/'

aceutils.copytreeWithExt(r'Sample/',targetDir+r'/Sample/',[ r'.h', r'.cpp', r'.filters', r'.vcxproj', r'.cs', r'.csproj', r'.sln', r'.wav', r'.ogg', r'.png'])

aceutils.mkdir(sampleDir+r'cpp/')
aceutils.mkdir(sampleDir+r'cpp/include/')
aceutils.mkdir(sampleDir+r'cpp/lib/')
aceutils.mkdir(sampleDir+r'cpp/lib/Debug/')
aceutils.mkdir(sampleDir+r'cpp/lib/Release/')

aceutils.copy(r'Dev/bin/ace_cs.dll', sampleDir+r'bin/')
aceutils.copy(r'Dev/bin/ace_core.Debug.dll', sampleDir+r'bin/')
aceutils.copy(r'Dev/bin/ace_core.dll', sampleDir+r'bin/')

aceutils.copy(r'Dev/include/ace.h', sampleDir+r'cpp/include/')
aceutils.copy(r'Dev/lib/Debug/ace_engine.lib', sampleDir+r'cpp/lib/Debug/')
aceutils.copy(r'Dev/lib/Release/ace_engine.lib', sampleDir+r'cpp/lib/Release/')

aceutils.copy(r'Dev/bin/ace_cs.dll', sampleDir+r'sample_cs/')
aceutils.copy(r'Dev/bin/ace_cs.XML', sampleDir+r'sample_cs/')

# Cpp
cpptargetDir=targetDir+r'/cpp/'
aceutils.mkdir(cpptargetDir)
aceutils.mkdir(cpptargetDir+r'Debug/')
aceutils.mkdir(cpptargetDir+r'Release/')

aceutils.copy(r'Dev/bin/ace_core.Debug.dll', cpptargetDir)
aceutils.copy(r'Dev/bin/ace_core.dll', cpptargetDir)
aceutils.copy(r'Dev/include/ace.h', cpptargetDir)
aceutils.copy(r'Dev/lib/Debug/ace_engine.lib',cpptargetDir+r'Debug/')
aceutils.copy(r'Dev/lib/Release/ace_engine.lib', cpptargetDir+r'Release/')

# CSharp
cstargetDir=targetDir+r'/cs/'
aceutils.mkdir(cstargetDir)
aceutils.copy(r'Dev/bin/ace_cs.dll', cstargetDir)
aceutils.copy(r'Dev/bin/ace_cs.XML', cstargetDir)

# Doxygen
aceutils.call(r'doxygen Script/acecppDoxyfile')
aceutils.copytree(r'Reference_CPP/html/', targetDir+r'/Reference_CPP')


