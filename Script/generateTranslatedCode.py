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
	exec_sync('.\..\Dev\generate_swig.py java')
	exec_sync('.\CSharpTranslator\LanguageTranslator.exe ../Dev/asd_cs/ ../Dev/asd_java/')
else:
	exec_sync('python ./../Dev/generate_swig.py java')
	exec_sync('mono ./CSharpTranslator/LanguageTranslator.exe ../Dev/asd_cs/ ../Dev/asd_java/')

