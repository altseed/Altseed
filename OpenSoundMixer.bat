
del master.zip
rmdir /S /Q OpenSoundMixer-master

python Script/ace_wget.py https://github.com/ac-engine/OpenSoundMixer/archive/master.zip
python Script/ace_unzip.py master.zip

cd OpenSoundMixer-master

echo compile
@Set Path=C:\Program Files (x86)\MSBuild\12.0\Bin;%PATH%
msbuild OpenSoundMixer.sln /p:configuration=Debug
msbuild OpenSoundMixer.sln /p:configuration=Release

cd ..

mkdir Dev\lib\Debug
mkdir Dev\lib\Release

copy OpenSoundMixer-master\src\OpenSoundMixer.h Dev\include\

copy OpenSoundMixer-master\lib\Debug\OpenSoundMixer.lib Dev\lib\Debug\
copy OpenSoundMixer-master\lib\Debug\libogg_static.lib Dev\lib\Debug\
copy OpenSoundMixer-master\lib\Debug\libvorbis_static.lib Dev\lib\Debug\
copy OpenSoundMixer-master\lib\Debug\libvorbisfile_static.lib Dev\lib\Debug\

copy OpenSoundMixer-master\lib\Release\OpenSoundMixer.lib Dev\lib\Release\
copy OpenSoundMixer-master\lib\Release\libogg_static.lib Dev\lib\Release\
copy OpenSoundMixer-master\lib\Release\libvorbis_static.lib Dev\lib\Release\
copy OpenSoundMixer-master\lib\Release\libvorbisfile_static.lib Dev\lib\Release\

pause
