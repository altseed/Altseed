#coding: UTF-8
import os.path
import aceutils

version = "1.0.0"

def genCS():
    targetDir = r'NuGetPackage/CSharp/'
    aceutils.cdToScript()
    aceutils.cd(r'../')
    
    # GenerateHeader
    aceutils.call(r'python Dev/generate_swig.py')

    aceutils.call(aceutils.cmd_compile + r'Dev/asd_cs.sln /p:configuration=Release')

    if not os.path.exists(targetDir):
        aceutils.mkdir(targetDir)
    aceutils.copy(r'Dev/bin/Altseed.dll', targetDir)
    aceutils.copy(r'Dev/bin/Altseed.xml', targetDir)
    aceutils.copy(r'Dev/bin/Altseed_core.dll', targetDir)

    aceutils.cd(targetDir)
    aceutils.call(r'nuget pack AltseedDotNet.nuspec -Version ' + version)

def genCPP():
    targetDir = r'NuGetPackage/Cpp/'
    aceutils.cdToScript()
    aceutils.cd(r'../')

    aceutils.call(aceutils.cmd_compile + r'Dev/asd_cpp.sln /p:configuration=Debug')    
    aceutils.call(aceutils.cmd_compile + r'Dev/asd_cpp.sln /p:configuration=Release')

    if not os.path.exists(targetDir):
        aceutils.mkdir(targetDir)

    aceutils.mkdir(targetDir+'build/native/bin/')
    aceutils.mkdir(targetDir+'build/native/include/')
    aceutils.mkdir(targetDir+'build/native/lib/')
    aceutils.mkdir(targetDir+'build/native/lib/Debug')
    aceutils.mkdir(targetDir+'build/native/lib/Release')

    aceutils.copy(r'Dev/bin/Altseed_core.dll', targetDir+'build/native/bin/')
    aceutils.copy(r'Dev/bin/Altseed_core_Debug.dll', targetDir+'build/native/bin/')
    aceutils.copy(r'Dev/include/Altseed.h', targetDir+'build/native/include/')
    aceutils.copy(r'Dev/lib/x86/Debug/Altseed.lib', targetDir+'build/native/lib/Debug/')
    aceutils.copy(r'Dev/lib/x86/Release/Altseed.lib', targetDir+'build/native/lib/Release/')

    aceutils.cd(targetDir)
    aceutils.call(r'nuget pack AltseedCpp.nuspec -Version ' + version)

if aceutils.isWin():
    genCS()
    genCPP()

else:
    print(r'NuGet package can be created only on Windows.')