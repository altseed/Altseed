sudo apt-get -y install autotools-dev
sudo apt-get -y install build-essential
sudo apt-get -y install automake
sudo apt-get -y install libtool

wget http://sourceforge.net/projects/swig/files/swig/swig-2.0.12/swig-2.0.12.tar.gz
tar zxvf swig-2.0.12.tar.gz
cd swig-2.0.12
./autogen.sh
./configure
make
sudo make install
