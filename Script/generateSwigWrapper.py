import subprocess
import sys
import os
import os.path

def exec_sync( cmd ):
	""" exec command line.
	"""

	print( cmd )
	p = subprocess.Popen(cmd, shell=True)
	ret = p.wait()
	print('')

exec_sync('.\CSharpWrapperGenerator\CSharpWrapperGenerator.exe CSharpWrapperGenerator\Settings.json')