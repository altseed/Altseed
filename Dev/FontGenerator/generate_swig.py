import subprocess
import os
import os.path
import sys


def conv(path):
	temp = open(path,mode='rb')
	byte = temp.read(1)
	temp.close()
	if b'\xef' == byte:
		return

	fin = open(path,mode='r',encoding='cp932')
	data = fin.read()
	fin.close()

	fout = open(path,mode='w',encoding='utf-8-sig')
	fout.write(data)
	fout.close()

def get_files(path):
	""" get files.
	"""

	def getlistdir(path,l):
		dirs = os.listdir(path)
		for d in dirs:
			newpath = os.path.join( path, d )
			if os.path.isdir( newpath ):
				getlistdir( newpath, l )
			else:
				l.append( newpath )
	
	ret = []
	getlistdir( path, ret )
	return ret


def conv_dir(d):
	files = get_files(d)
	for file in files:
		root, ext = os.path.splitext(file)
		if not ext in ['.cpp','.h']:
			continue
		conv(file)


def exec_sync( cmd ):
	""" exec command line.
	"""

	print( cmd )
	p = subprocess.Popen(cmd, shell=True)
	ret = p.wait()
	print('')

if not os.path.isdir('FontGenerator/swig'):
  os.makedirs('FontGenerator/swig')

if not os.path.isdir('FontGenerator.Model/swig'):
  os.makedirs('FontGenerator.Model/swig')

exec_sync( 'swig -c++ -csharp -namespace FontGenerator.swig -dllimport FontGeneratorCore -o FontGeneratorCore/dll_cs.cxx -outdir FontGenerator/swig/ FontGenerator.i' )
exec_sync( 'swig -c++ -csharp -namespace FontGenerator.swig -dllimport FontGeneratorCore -o FontGeneratorCore/dll_cs.cxx -outdir FontGenerator.Model/swig/ FontGenerator.i' )
