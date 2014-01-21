rm -rf glfw_bin
rm -rf amcr-glfw

mkdir glfw_bin
mkdir amcr-glfw

cp -r Dev/amcr-glfw/ ./

python Script/ace_editCmakeForACE.py amcr-glfw/CMakeLists.txt

(cd glfw_bin;
cmake -G "Unix Makefiles" -D BUILD_SHARED_LIBS:BOOL=OFF -D CMAKE_INSTALL_PREFIX=../Dev ../amcr-glfw/;
cd glfw_bin;
make install;)
