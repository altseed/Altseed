
import aceutils

aceutils.cd(r'../')

aceutils.rmdir(r'Dev/cmake')
aceutils.mkdir(r'Dev/cmake')

aceutils.cd(r'Dev/cmake')

if aceutils.isWin():
	aceutils.call(r'cmake -G "Visual Studio 12" -D BUILD_SHARED_LIBS:BOOL=OFF -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ../')
else:
	aceutils.call(r'cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF ../')
	aceutils.call(r'make install')
	aceutils.cd(r'../../')
	aceutils.copytree(r'Dev/bin/Data',r'Dev/cmake/bin')

