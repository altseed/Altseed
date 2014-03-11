import sys
import os
import shutil
import subprocess
import os
import os.path

def exec_sync( cmd ):
	""" exec command line.
	"""

	print( cmd )
	p = subprocess.Popen(cmd, shell=True)
	ret = p.wait()
	print('')

argvs = sys.argv
argc = len(argvs)

if argc >= 2:
	exec_sync(argvs[1])
