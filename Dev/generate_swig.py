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

exec_sync( '..\\ConvertCharactorSet.exe ace_cpp/ *.cpp CP932' )
exec_sync( '..\\ConvertCharactorSet.exe ace_cpp/ *.h CP932' )

exec_sync( '..\\ConvertCharactorSet.exe unitTest_cpp_gtest/ *.cpp CP932' )
exec_sync( '..\\ConvertCharactorSet.exe unitTest_cpp_gtest/ *.h CP932' )

exec_sync( 'mkdir ace_cs\\swig' )
exec_sync( 'swig -c++ -csharp -namespace ace.swig -dllimport ace_core -o ace_cpp\\core\\dll_cs.cxx -outdir ace_cs\\swig\\ ace_cs.i' )

exec_sync( '..\\ConvertCharactorSet.exe ace_cpp/ *.cpp UTF8' )
exec_sync( '..\\ConvertCharactorSet.exe ace_cpp/ *.h UTF8' )

exec_sync( '..\\ConvertCharactorSet.exe unitTest_cpp_gtest/ *.cpp UTF8' )
exec_sync( '..\\ConvertCharactorSet.exe unitTest_cpp_gtest/ *.h UTF8' )