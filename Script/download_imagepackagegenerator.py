
import aceutils

aceutils.cdToScript()
aceutils.cd(r'../')
aceutils.rm(r'master.zip')
aceutils.rmdir(r'ImagePackageGenerator-master')

aceutils.wget(r'https://github.com/altseed/ImagePackageGenerator/archive/master.zip')
aceutils.unzip(r'master.zip')

aceutils.cd(r"ImagePackageGenerator-master")

if aceutils.isWin():
	aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" ImagePackageGenerator.sln /p:configuration=Release')

aceutils.cd(r"../")

aceutils.mkdir(r'Dev/bin/')
aceutils.copy(r'ImagePackageGenerator-master/bin/ImagePackageGenerator.exe', r'Dev/bin/')
aceutils.copy(r'ImagePackageGenerator-master/bin/ImagePackageGenerator.exe.config', r'Dev/bin/')
aceutils.copy(r'ImagePackageGenerator-master/bin/PSDParser.dll', r'Dev/bin/')


