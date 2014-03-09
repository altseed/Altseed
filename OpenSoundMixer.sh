
rm master.zip
rm -rf OpenSoundMixer-master

python Script/ace_wget.py https://github.com/ac-engine/OpenSoundMixer/archive/master.zip
python Script/ace_unzip.py master.zip

python Script/ace_editCmakeForACE.py OpenSoundMixer-master/CMakeLists.txt

rm -rf osm_bin
mkdir osm_bin

(cd osm_bin
cmake -G "Unix Makefiles" ../OpenSoundMixer-master/
make;)

cp OpenSoundMixer-master/src/OpenSoundMixer.h Dev/include/

cp osm_bin/lib/libOpenSoundMixer.a Dev/lib/
