import os.path
import sys
import aceutils

def init():
	aceutils.cdToScript()
	aceutils.cd(r'../')

	if aceutils.isWin():
		aceutils.call(r'python Dev/generate_swig.py')
		
	elif aceutils.isMac():
		# generate header(C#)
		aceutils.call(r'python Dev/generate_swig.py')

		aceutils.rmdir(r'Dev/cmake')
		aceutils.mkdir(r'Dev/cmake')
		aceutils.cd(r'Dev/cmake')
		aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX:PATH=../ "-DCMAKE_OSX_ARCHITECTURES=x86_64;i386" ../')
		aceutils.call(r'make install')
		aceutils.cd(r'../../')
		aceutils.copytree(r'Dev/bin/Data',r'Dev/cmake/bin/Data')

	else:
		# generate header(C#)
		aceutils.call(r'python Dev/generate_swig.py')

		aceutils.rmdir(r'Dev/cmake')
		aceutils.mkdir(r'Dev/cmake')
		aceutils.cd(r'Dev/cmake')
		aceutils.call(r'cmake -G "Unix Makefiles" -D CMAKE_BUILD_TYPE=Release -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX:PATH=../ ../')
		aceutils.call(r'make install')
		aceutils.cd(r'../../')
		aceutils.copytree(r'Dev/bin/Data',r'Dev/cmake/bin/Data')

def compile(type):
	aceutils.cdToScript()

	if type=='cpp':
		aceutils.call(r'python ../Dev/generate_swig.py')
	if type=='cs':
		aceutils.call(r'python ../Dev/generate_swig.py')
	if type=='java':
		aceutils.call(r'python ../Dev/generate_swig.py java')
		aceutils.call('.\CSharpTranslator\LanguageTranslator.exe ../Dev/asd_cs/ ../Dev/asd_java/')

	aceutils.cd(r'../')

	if aceutils.isWin():
		if type=='cpp':
			aceutils.call(aceutils.cmd_compile + r'Dev/unitTest_Engine_cpp.sln /p:configuration=Debug')
			aceutils.call(aceutils.cmd_compile + r'Dev/unitTest_Engine_cpp.sln /p:configuration=Release')
		
		if type=='cs':
			aceutils.call(aceutils.cmd_compile + r'Dev/unitTest_Engine_cs.sln /p:configuration=Release')

		if type=='java':
			aceutils.call(aceutils.cmd_compile + r'Dev/unitTest_Engine_cpp.sln /p:configuration=Release /p:platform=x64')
			aceutils.cd(r'Dev/asd_java')
			aceutils.call(r'ant')
			aceutils.cd(r'../../')

	elif aceutils.isMac():
		if type=='cpp':
			aceutils.cd(r'Dev/cmake')
			aceutils.call(r'make install')
			aceutils.cd(r'../../')

		elif type=='cs':
			aceutils.cd(r'Dev')
			aceutils.call(r'xbuild /p:Configuration=Release unitTest_Engine_cs.sln')
			aceutils.cd(r'../')

		elif type=='java':
			aceutils.cd(r'Dev/cmake')
			aceutils.call(r'make install')
			aceutils.cd(r'../../')

			aceutils.cd(r'Dev/asd_java')
			aceutils.call(r'ant')
			aceutils.cd(r'../../')

if __name__ == "__main__":
	init()
	compile('cs')