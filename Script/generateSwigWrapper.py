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
	exec_sync('.\CSharpWrapperGenerator\CSharpWrapperGenerator.exe CSharpWrapperGenerator\Settings.json')
else:
	exec_sync('mono ./CSharpWrapperGenerator/CSharpWrapperGenerator.exe CSharpWrapperGenerator/Settings.json')
