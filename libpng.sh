rm -rf zlib_bin
rm -rf libpng_bin

wget http://downloads.sourceforge.net/project/libpng/libpng16/1.6.6/libpng-1.6.6.tar.gz
wget http://zlib.net/zlib-1.2.8.tar.gz

tar zxvf libpng-1.6.6.tar.gz
tar zxvf zlib-1.2.8.tar.gz

mkdir zlib_bin
(cd zlib_bin;
cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX=../Dev ../zlib-1.2.8/;
make install;)

mkdir libpng
(cd libpng;
cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX=../Dev ../libpng-1.6.6/;
make install;)

rm Dev/lib/libz.so
rm Dev/lib/libz.so.1
rm Dev/lib/libz.so.1.2.8

rm Dev/lib/libpng.so
rm Dev/lib/libpng16.so
rm Dev/lib/libpng16.so.16
rm Dev/lib/libpng16.so.16.6.0
