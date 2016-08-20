import os
import os.path
import aceutils

aceutils.cdToScript()
aceutils.cd(r'../')

aceutils.rmdir(r'libogg-1.3.2')
aceutils.rmdir(r'libvorbis-1.3.5')

aceutils.wget(r'http://downloads.xiph.org/releases/ogg/libogg-1.3.2.tar.gz')
aceutils.wget(r'http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.5.tar.gz')

aceutils.call(r'tar zxvf libogg-1.3.2.tar.gz')
aceutils.call(r'tar zxvf libvorbis-1.3.5.tar.gz')

if aceutils.isWin():
	pass
else:
	ogg_dir="libogg-1.3.2"
	aceutils.cd(ogg_dir)
	cmdenv=""
	if aceutils.isMac():
		cmdenv+="CFLAGS='-arch i386 -arch x86_64 -fPIC '$CFLAGS "
	else:
		cmdenv+="CFLAGS='-fPIC '$CFLAGS "
	aceutils.call(cmdenv+"./configure --prefix=`pwd`/build --disable-shared")
	aceutils.call(cmdenv+"make")
	aceutils.call(cmdenv+"make install")
	ogg_build_dir=os.getcwd()+"/build"
	aceutils.cd("../")

	vorbis_dir="libvorbis-1.3.5"
	aceutils.cd(vorbis_dir)
	cmdenv=""
	if aceutils.isMac():
		cmdenv+="CFLAGS='-arch i386 -arch x86_64 -fPIC '$CFLAGS "
	else:
		cmdenv+="CFLAGS='-fPIC '$CFLAGS "
	cmdenv+="PKG_CONFIG_PATH="+ogg_build_dir+"/lib/pkgconfig:$PKG_CONFIG_PATH "
	aceutils.call(cmdenv+"./configure --prefix=`pwd`/build -disable-shared --with-ogg="+ogg_build_dir)
	aceutils.call(cmdenv+"make")
	aceutils.call(cmdenv+"make install")
	aceutils.cd("../")

	aceutils.mkdir(r'Dev/include/ogg')
	aceutils.mkdir(r'Dev/include/vorbis')

	aceutils.copy(ogg_dir+r'/build/include/ogg/ogg.h',r'Dev/include/ogg')
	aceutils.copy(ogg_dir+r'/build/include/ogg/os_types.h',r'Dev/include/ogg')
	aceutils.copy(ogg_dir+r'/build/include/ogg/config_types.h',r'Dev/include/ogg')
	aceutils.copy(vorbis_dir+r'/build/include/vorbis/codec.h',r'Dev/include/vorbis')
	aceutils.copy(vorbis_dir+r'/build/include/vorbis/vorbisfile.h',r'Dev/include/vorbis')
	aceutils.copy(vorbis_dir+r'/build/include/vorbis/vorbisenc.h',r'Dev/include/vorbis')

	aceutils.copy(ogg_dir+r'/build/lib/libogg.a',r'Dev/lib')
	aceutils.copy(vorbis_dir+r'/build/lib/libvorbis.a',r'Dev/lib')
	aceutils.copy(vorbis_dir+r'/build/lib/libvorbisfile.a',r'Dev/lib')
