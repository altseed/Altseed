import os.path
import aceutils
import compile
import sys

type = 'cs'
if len(sys.argv) == 2:
	type = sys.argv[1]

if len(sys.argv) == 3:
	type = sys.argv[2]

compile.init()

if type=='java':
	compile.compile('cs')
	
compile.compile(type)

aceutils.cdToScript()
aceutils.cd(r'../')

def copyDev(targetDir):
	aceutils.mkdir(targetDir+r'/')

	# Sample
	sampleDir = targetDir+r'/'

	if type == 'cpp':
		aceutils.mkdir(sampleDir+r'cpp/')
		aceutils.mkdir(sampleDir+r'cpp/include/')
		aceutils.mkdir(sampleDir+r'cpp/lib/')
		aceutils.mkdir(sampleDir+r'cpp/lib/Debug/')
		aceutils.mkdir(sampleDir+r'cpp/lib/Release/')

		aceutils.copy(r'Dev/include/Altseed.h', sampleDir+r'cpp/include/')

		if aceutils.isWin():
			aceutils.copy(r'Dev/lib/x86/Debug/Altseed.lib', sampleDir+r'cpp/lib/Debug/')
			aceutils.copy(r'Dev/lib/x86/Release/Altseed.lib', sampleDir+r'cpp/lib/Release/')
		else:
			aceutils.copy(r'Dev/lib/libAltseed.a', sampleDir+r'cpp/lib/')

		if aceutils.isWin():
			aceutils.copy(r'Dev/bin/Altseed_core_Debug.dll', sampleDir+r'bin/')
			aceutils.copy(r'Dev/bin/Altseed_core.dll', sampleDir+r'bin/')
		elif aceutils.isMac():
			aceutils.copy(r'Dev/bin/libAltseed_core.dylib', sampleDir+r'bin/')
		else:
			aceutils.copy(r'Dev/bin/libAltseed_core.so', sampleDir+r'bin/')

	if type == 'cs':
		aceutils.copy(r'Dev/bin/Altseed.dll', sampleDir+r'bin/')
		if aceutils.isWin():
			aceutils.copy(r'Dev/bin/Altseed_core.dll', sampleDir+r'bin/')
		elif aceutils.isMac():
			aceutils.copy(r'Dev/bin/libAltseed_core.dylib', sampleDir+r'bin/')
		else:
			aceutils.copy(r'Dev/bin/libAltseed_core.so', sampleDir+r'bin/')

	if type == 'java':
		aceutils.copy(r'Dev/bin/Altseed.dll', sampleDir+r'bin/')
		aceutils.copy(r'Dev/bin/Altseed.jar', sampleDir+r'bin/')

		if aceutils.isWin():
			aceutils.call('.\Script\CSharpTranslator\LanguageTranslator.exe '+sampleDir+r'sample_cs/ '+sampleDir+r'sample_java/ java '+sampleDir+r'bin/Altseed.dll')
		else:
			aceutils.call('mono ./Script/CSharpTranslator/LanguageTranslator.exe '+sampleDir+r'sample_cs/ '+sampleDir+r'sample_java/ java '+sampleDir+r'bin/Altseed.dll')

		if aceutils.isWin():
			aceutils.copy(r'Dev/bin/Altseed_core.dll', sampleDir+r'bin/')
		elif aceutils.isMac():
			aceutils.copy(r'Dev/bin/libAltseed_core.dylib', sampleDir+r'bin/')
		else:
			aceutils.copy(r'Dev/bin/libAltseed_core.so', sampleDir+r'bin/')


copyDev(r'Sample/BasicSample')
copyDev(r'Sample/ApplicationSample')

