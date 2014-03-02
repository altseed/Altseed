
del master.zip
rm -rf Effekseer-master

python Script/ace_wget.py https://github.com/effekseer/Effekseer/archive/master.zip
python Script/ace_unzip.py master.zip

python Script/ace_editCmakeForACE.py Effekseer-master/Dev/Cpp/CMakeLists.txt

rm -rf effekseer_bin
mkdir effekseer_bin

(cd effekseer_bin
cmake -G "Unix Makefiles" -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF -D USE_INTERNAL_LOADER:BOOL=OFF ../Effekseer-master/Dev/Cpp/
make;)

cp Effekseer-master/Dev/Cpp/Effekseer/Effekseer.h Dev/include/
cp Effekseer-master/Dev/Cpp/EffekseerRendererGL/EffekseerRendererGL.h Dev/include/
cp effekseer_bin/libEffekseer.a Dev/lib/
cp effekseer_bin/libEffekseerRendererGL.a Dev/lib/

