#coding: UTF-8
import os.path
import aceutils

if aceutils.isWin():
    version = input('input version:')
    targetDir = r'NuGetPackage'
    aceutils.cdToScript()
    aceutils.cd(r'../')
    
    aceutils.call(aceutils.cmd_compile + r'Dev/asd_cs.sln /p:configuration=Release')

    if not os.path.exists(targetDir):
        aceutils.mkdir(targetDir)
    aceutils.copy(r'Dev/bin/Altseed.dll', targetDir)
    aceutils.copy(r'Dev/bin/Altseed.xml', targetDir)
    aceutils.copy(r'Dev/bin/Altseed_core.dll', targetDir)

    aceutils.cd(targetDir)
    aceutils.call(r'nuget pack Altseed.nuspec -Version ' + version)
else:
    print(r'NuGet package can be created only on Windows.')