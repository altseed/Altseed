echo オプション不足により使い物にならず　要更新待ち
pause

rmdir /S /Q zlib_bin
rmdir /S /Q libpng_bin

#wget http://downloads.sourceforge.net/project/libpng/libpng16/1.6.3/lpng163.zip
#wget http://zlib.net/zlib128.zip

#unzip lpng163.zip
#unzip zlib128.zip

mkdir zlib_bin
cd zlib_bin
cmake -G "Visual Studio 12" -D BUILD_SHARED_LIBS:BOOL=OFF -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ../zlib-1.2.8/
@Set Path=C:\Program Files (x86)\MSBuild\12.0\Bin;%PATH%
msbuild src/zlib.sln /p:configuration=Debug
msbuild src/zlib.sln /p:configuration=Release

mkdir libpng

pause

