import subprocess
import sys
import os
import os.path
import aceutils

def exec_sync( cmd ):
	""" exec command line.
	"""

	print( cmd )
	p = subprocess.Popen(cmd, shell=True)
	ret = p.wait()
	print('')

aceutils.cdToScript()

if aceutils.isWin():
	exec_sync('.\CSharpTranslator\LanguageTranslator.exe ../Sample/BasicSample/sample_cs/ ../Sample/BasicSample/sample_java/ java ../Sample/BasicSample/bin/Altseed.dll')
else:
	exec_sync('mono ./CSharpTranslator/LanguageTranslator.exe ../Sample/BasicSample/sample_cs/ ../Sample/BasicSample/sample_java/ java ../Sample/BasicSample/bin/Altseed.dll')

