import subprocess
import sys
import os
import os.path

def cd(path):
	os.chdir(path)

def cdToScript():
	cd(os.path.dirname(os.path.abspath(__file__)))

def conv(path):
	print('Convert {}'.format(path))
	temp = open(path,mode='rb')
	byte = temp.read(1)
	temp.close()
	if b'\xef' == byte:
		return

	data = None
	
	if data is None:
		try:
			fin = open(path,mode='r',encoding='cp932')
			data = fin.read()
			fin.close()
		except:
			pass

	if data is None:
		try:
			fin = open(path,mode='r',encoding='utf-8')
			data = fin.read()
			fin.close()
		except:
			pass

	if data is None:
		raise Exception('invalid encode')

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

conv_dir(r'asd_cpp/')
conv_dir(r'unitTest_cpp_gtest/')
conv_dir(r'unitTest_Engine_cpp_gtest/')

argv = sys.argv

if len(argv) == 1 or argv[1] == 'csharp':
	if not os.path.isdir('asd_cs/swig'):
		os.makedirs('asd_cs/swig')

	exec_sync( 'swig -c++ -csharp -namespace asd.swig -dllimport Altseed_core -o asd_cpp/core/dll.cxx -outdir asd_cs/swig/ swig.i' )


	f = open(r'asd_cs/swig/asd_corePINVOKE.cs', 'r', encoding='utf-8')
	lines = f.read()
	f.close()

	lines = lines.replace(r'"Altseed_core"', r'asd.Particular.Define.DLL')

	f = open(r'asd_cs/swig/asd_corePINVOKE.cs', 'w')
	f.write(lines)
	f.close()




elif argv[1] == 'java':
	if not os.path.isdir('asd_java'):
		os.makedirs('asd_java')
	if not os.path.isdir('asd_java/swig'):
		os.makedirs('asd_java/swig')
	exec_sync( 'swig -c++ -java -package asd.swig -o asd_cpp/core/dll.cxx -outdir asd_java/swig/ swig.i' )