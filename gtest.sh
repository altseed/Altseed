
wget https://googletest.googlecode.com/files/gtest-1.7.0.zip
unzip gtest-1.7.0.zip

mkdir gtest_bin
(cd gtest_bin;
cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX=../Dev ../gtest-1.7.0/;
make;)

cp gtest_bin/libgtest.a Dev/lib/
cp gtest_bin/libgtest_main.a Dev/lib/

cp -rp gtest-1.7.0/include/gtest Dev/include/

