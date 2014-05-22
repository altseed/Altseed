sudo apt-get -y install autotools-dev
sudo apt-get -y install build-essential
sudo apt-get -y install automake
sudo apt-get -y install libtool
sudo apt-get -y install libpcre3-dev

wget http://sourceforge.net/projects/swig/files/swig/swig-3.0.0/swig-3.0.0.tar.gz
tar zxvf swig-3.0.0.tar.gz
cd swig-3.0.0
./autogen.sh
./configure
make
sudo make install
