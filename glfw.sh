rm -rf glfw_bin
mkdir glfw_bin
(cd glfw_bin;
cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX=../Dev ../Dev/amcr-glfw/;
cd glfw_bin;
make install;)