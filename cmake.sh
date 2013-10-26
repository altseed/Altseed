rm -rf Dev/cmake
mkdir Dev/cmake

(cd Dev/cmake;
cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF ../;
make install;)
