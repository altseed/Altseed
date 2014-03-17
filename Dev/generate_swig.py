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

conv_dir(r'ace_cpp/')
conv_dir(r'unitTest_cpp_gtest/')
conv_dir(r'unitTest_Engine_cpp_gtest/')

if not os.path.isdir('ace_cs/swig'):
  os.makedirs('ace_cs/swig')


exec_sync( 'swig -c++ -csharp -namespace ace.swig -dllimport ace_core -o ace_cpp/core/dll_cs.cxx -outdir ace_cs/swig/ ace_cs.i' )
