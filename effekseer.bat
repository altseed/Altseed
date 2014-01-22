python Script/ace_wget.py https://github.com/effekseer/Effekseer/archive/master.zip
python Script/ace_unzip.py master.zip
copy Effekseer-master\CMakeLists.txt Effekseer-master\Dev\Cpp\

rmdir /S /Q effekseer_bin
mkdir effekseer_bin
cd effekseer_bin
cmake -G "Visual Studio 12" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../Effekseer-master/Dev/Cpp/

echo compile
@Set Path=C:\Program Files (x86)\MSBuild\12.0\Bin;%PATH%
msbuild Effekseer.sln /p:configuration=Debug
msbuild Effekseer.sln /p:configuration=Release
