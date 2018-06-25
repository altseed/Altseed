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
	exec_sync('.\CppWrapperGenerator\CppWrapperGenerator.exe CppWrapperGenerator\Settings.json')
else:
	exec_sync('mono ./CppWrapperGenerator/CppWrapperGenerator.exe CppWrapperGenerator/Settings.json')
