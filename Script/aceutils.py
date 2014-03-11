import sys
import os
import shutil
import subprocess
import os
import os.path
import urllib.request
import zipfile
import platform

def isWin():
	return platform.system() == 'Windows'

def call( cmd ):
	""" call command line.
	"""

	print( cmd )
	p = subprocess.Popen(cmd, shell=True)
	ret = p.wait()
	print('')


def wget( address ):
	urllib.request.urlretrieve(address, os.path.basename(address))


def unzip(zip_filename):
	zip_file = zipfile.ZipFile(zip_filename, "r")
	for f in zip_file.namelist():

		if f.endswith('/'):
		        os.makedirs(f)
		else:
			unzip_file = open(f, "wb")
			unzip_file.write(zip_file.read(f))
			unzip_file.close();
	zip_file.close()


def rm(path):
	if os.path.exists(path):
		os.remove(path)

def rmdir(path):
	if os.path.exists(path):
		shutil.rmtree(path)

def cd(path):
	os.chdir(path)

def mkdir(path):
	os.mkdir(path)

def copy(src,dst):
	shutil.copyfile(src,dst)

def copytree(src,dst):
	shutil.copytree(src,dst)


