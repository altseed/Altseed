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


exec_sync( 'swig -c++ -csharp -namespace ImagePackageGenerator.swig -dllimport PSDParser -o PSDParser\\dll.cxx -outdir ImagePackageGenerator\\swig\\ swig.i' )

