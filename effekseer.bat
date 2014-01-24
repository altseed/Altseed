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

cd ..

mkdir Dev\lib\Debug
mkdir Dev\lib\Release

copy Effekseer-master\Dev\Cpp\Effekseer\Effekseer.h Dev\include\
copy Effekseer-master\Dev\Cpp\EffekseerRendererDX11\EffekseerRendererDX11.h Dev\include\
copy Effekseer-master\Dev\Cpp\EffekseerRendererGL\EffekseerRendererGL.h Dev\include\
copy effekseer_bin\Debug\Effekseer.lib Dev\lib\Debug\
copy effekseer_bin\Debug\EffekseerRendererDX11.lib Dev\lib\Debug\
copy effekseer_bin\Debug\EffekseerRendererGL.lib Dev\lib\Debug\
copy effekseer_bin\Release\Effekseer.lib Dev\lib\Release\
copy effekseer_bin\Release\EffekseerRendererDX11.lib Dev\lib\Release\
copy effekseer_bin\Release\EffekseerRendererGL.lib Dev\lib\Release\


pause
