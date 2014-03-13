
import aceutils

targetDir = r'ACE140315'

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
aceutils.copytreeWithExt(r'Sample/',targetDir+r'/Sample/',[ r'.h', r'.cpp', r'.filters', r'.vcxproj', r'.cs', r'.csproj', r'.sln', r'.wav', r'.ogg', r'.png'])

aceutils.mkdir(targetDir+r'/Sample/cpp/include/')
aceutils.mkdir(targetDir+r'/Sample/cpp/lib/')
aceutils.mkdir(targetDir+r'/Sample/cpp/lib/Debug/')
aceutils.mkdir(targetDir+r'/Sample/cpp/lib/Release/')

aceutils.copy(r'Dev/bin/ace_cs.dll', targetDir+r'/Sample/bin/')
aceutils.copy(r'Dev/bin/ace_core.Debug.dll', targetDir+r'/Sample/bin/')
aceutils.copy(r'Dev/bin/ace_core.dll', targetDir+r'/Sample/bin/')

aceutils.copy(r'Dev/include/ace.h', targetDir+r'/Sample/cpp/include/')
aceutils.copy(r'Dev/lib/Debug/ace_engine.lib', targetDir+r'/Sample/cpp/lib/Debug/')
aceutils.copy(r'Dev/lib/Release/ace_engine.lib', targetDir+r'/Sample/cpp/lib/Release/')

aceutils.copy(r'Dev/bin/ace_cs.dll', targetDir+r'/Sample/sample_cs/')
aceutils.copy(r'Dev/bin/ace_cs.XML', targetDir+r'/Sample/sample_cs/')

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




