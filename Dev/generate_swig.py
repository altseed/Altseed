import subprocess
import sys
import os
import os.path

def cd(path):
	os.chdir(path)

def cdToScript():
	cd(os.path.dirname(__file__))

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

cdToScript()

conv_dir(r'ace_cpp/')
conv_dir(r'unitTest_cpp_gtest/')
conv_dir(r'unitTest_Engine_cpp_gtest/')

argv = sys.argv

if len(argv) == 1 or argv[1] == 'csharp':
	if not os.path.isdir('ace_cs/swig'):
		os.makedirs('ace_cs/swig')

	exec_sync( 'swig -c++ -csharp -namespace ace.swig -dllimport ace_core -o ace_cpp/core/dll.cxx -outdir ace_cs/swig/ swig.i' )

elif argv[1] == 'java':
	if not os.path.isdir('ace_java'):
		os.makedirs('ace_java')
	if not os.path.isdir('ace_java/swig'):
		os.makedirs('ace_java/swig')
	exec_sync( 'swig -c++ -java -package ace.swig -o ace_cpp/core/dll.cxx -outdir ace_java/swig/ swig.i' )