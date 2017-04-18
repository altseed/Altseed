GIT?=$(shell which git)
CMAKE?=$(shell which cmake)
PYTHON?=$(shell which python3)
XBUILD?=$(shell which xbuild)
ANT?=$(shell which ant)

all: cpp csharp java

submodule: 
	$(GIT) submodule update --init --recursive

clean-absolute:
	$(GIT) clean -df
	$(GIT) reset --hard HEAD


GLFW_LIB=Downloads/glfw_bin/src/libglfw3.a
glfw: submodule $(GLFW_LIB)

glfw-clean:
	cd Downloads/glfw_bin; $(MAKE) clean
	cd Downloads/glfw; git reset --hard HEAD

$(GLFW_LIB):
	$(PYTHON) Script/build_glfw.py


LIBPNG_LIB=Downloads/libpng_bin/libpng16.a
libpng: submodule $(LIBPNG_LIB)

libpng-clean:
	cd Downloads/libpng_bin; $(MAKE) clean
	cd Downloads/libpng; git reset --hard HEAD

$(LIBPNG_LIB):
	$(PYTHON) Script/build_libpng.py


BOX2D_LIB=Downloads/box2d_bin/Box2D/libBox2D.a
box2d: submodule $(BOX2D_LIB)

box2d-clean:
	cd Downloads/box2d_bin; $(MAKE) clean
	cd Downloads/Box2D; git reset --hard HEAD 
	cd Downloads/Box2D-Linux; git reset --hard HEAD 

$(BOX2D_LIB):
	$(PYTHON) Script/build_Box2D.py


BULLET_LIB=Downloads/bullet_bin/src/BulletCollision/libBulletCollision.a
bullet: submodule $(BULLET_LIB)

bullet-clean:
	cd Downloads/bullet_bin; $(MAKE) clean
	cd Downloads/bullet3; git reset --hard HEAD

$(BULLET_LIB):
	$(PYTHON) Script/build_bullet.py


GOOGLETEST_LIB=Downloads/gtest_bin/libgtest.a
googletest: submodule $(GOOGLETEST_LIB)

googletest-clean:
	cd Downloads/gtest_bin; $(MAKE) clean
	cd Downloads/googletest; git reset --hard HEAD

$(GOOGLETEST_LIB):
	$(PYTHON) Script/build_gtest.py
	

EFFEKSEER_LIB=Downloads/effekseer_bin/libEffekseer.a
effekseer: submodule $(EFFEKSEER_LIB)

effekseer-clean:
	cd Downloads/effekseer_bin; $(MAKE) clean
	cd Downloads/Effekseer; $(GIT) reset --hard HEAD

$(EFFEKSEER_LIB):
	$(PYTHON) Script/build_effekseer.py
	

OPENSOUNDMIXER_LIB=Downloads/osm_bin/lib/libOpenSoundMixer.a
opensoundmixer: submodule $(OPENSOUNDMIXER_LIB)

opensoundmixer-clean:
	cd Downloads/osm_bin; $(MAKE) clean
	cd Downloads/OpenSoundMixer; $(GIT) reset --hard HEAD

$(OPENSOUNDMIXER_LIB):
	$(PYTHON) Script/build_OpenSoundMixer.py


FREETYPE_LIB=Downloads/freetype_bin/libfreetype.a
freetype: submodule $(FREETYPE_LIB)

freetype-clean:
	cd Downloads/freetype_bin; $(MAKE) clean
	cd Downloads/freetype; $(GIT) reset --hard HEAD

$(FREETYPE_LIB):
	$(PYTHON) Script/build_freetype.py
	

LIBGD_LIB=Downloads/gd_bin/Bin/libgd.a
libgd: submodule $(LIBGD_LIB)

libgd-clean:
	cd Downloads/gd_bin; $(MAKE) clean
	cd Downloads/libgd; $(GIT) reset --hard HEAD

$(LIBGD_LIB):
	$(PYTHON) Script/build_libgd.py
	

CULLING2D_LIB=Downloads/culling2d_bin/bin/libCulling2D.a
culling2d: submodule $(CULLING2D_LIB)

culling2d-clean:
	cd Downloads/culling2d_bin; $(MAKE) clean
	cd Downloads/Culling2D; $(GIT) reset --hard HEAD

$(CULLING2D_LIB):
	$(PYTHON) Script/build_culling2d.py


CULLING3D_LIB=Downloads/culling3d_bin/libCulling3D.a
culling3d: submodule $(CULLING3D_LIB)

culling3d-clean:
	cd Downloads/culling3d_bin; $(MAKE) clean
	cd Downloads/Culling3D; $(GIT) reset --hard HEAD

$(CULLING3D_LIB):
	$(PYTHON) Script/build_culling3d.py


ALTSEEDPLATFORM_LIB=Downloads/AltseedPlatform_bin/src/libAltseedPlatform.a
altseedplatform: submodule $(ALTSEEDPLATFORM_LIB)

altseedplatform-clean:
	cd Downloads/AltseedPlatform_bin; $(MAKE) clean
	cd Downloads/AltseedPlatform; $(GIT) reset --hard HEAD

$(ALTSEEDPLATFORM_LIB):
	$(PYTHON) Script/build_ap.py


ALTSEEDRHI_LIB=Downloads/AltseedRHI_bin/src/libAltseedRHI.a
altseedrhi: submodule $(ALTSEEDRHI_LIB)

altseedrhi-clean:
	cd Downloads/AltseedRHI_bin; $(MAKE) clean
	cd Downloads/AltseedRHI; $(GIT) reset --hard HEAD

$(ALTSEEDRHI_LIB):
	$(PYTHON) Script/build_ar.py


plugins-all: box2d bullet culling2d culling3d effekseer freetype glfw googletest libgd libpng opensoundmixer altseedplatform altseedrhi

clean-all-plugins: box2d-clean bullet-clean culling2d-clean culling3d-clean effekseer-clean freetype-clean glfw-clean googletest-clean libgd-clean libpng-clean opensoundmixer-clean altseedplatform-clean altseedrhi-clean


cpp-prepare: plugins-all ENVVAR_JAVA_HOME Dev/cmake/Makefile 
	$(PYTHON) Dev/generateCoreToEngineHeader.py
	$(PYTHON) Dev/generateEngineHeader.py
	$(PYTHON) Dev/generate_swig.py

Dev/cmake/Makefile: 
	mkdir -p Dev/cmake/bin/Data
	cd Dev/cmake ; \
	$(CMAKE) -G "Unix Makefiles" \
		-D BUILD_SHARED_LIBS:BOOL=OFF \
		-D CMAKE_INSTALL_PREFIX:PATH=../ \
		../ ;


csharp-prepare: plugins-all cpp-prepare Doxygen
	$(PYTHON) Script/generateSwigWrapper.py

Doxygen:
	$(PYTHON) Script/export_doxygen_core.py

java-prepare: plugins-all cpp-prepare Dev/asd_java/asd/AltseedObject.java 
	$(PYTHON) Dev/generate_swig.py java

Dev/asd_java/asd/AltseedObject.java:
	$(PYTHON) Script/generateTranslatedCode.py


jh=$(shell readlink -f /usr/bin/javac | sed "s:/bin/javac::")
ENVVAR_JAVA_HOME:
ifndef JAVA_HOME
	$(warning environment variable 'JAVA_PATH' is not defined, setting "$(jh)".)
	@ export JAVA_HOME=$(jh)
endif


cpp: cpp-prepare Dev/lib/libAltseed.a

Dev/lib/libAltseed.a: 
	cd Dev/cmake ; $(MAKE) install
	cp -r Dev/bin/Data/* Dev/cmake/bin/Data/


csharp: csharp-prepare Dev/lib/libAltseed.a Dev/bin/Altseed.dll

Dev/bin/Altseed.dll:
	cd Dev; \
	$(XBUILD) /p:Configuration=Release unitTest_Engine_cs.sln


java: java-prepare Dev/lib/libAltseed.a Dev/bin/Altseed.jar

Dev/bin/Altseed.jar:
	cd Dev/asd_java/ ; $(ANT)


sample-cpp: cpp
	$(PYTHON) Script/generate_sample.py cpp

sample-csharp: csharp
	$(PYTHON) Script/generate_sample.py cs

sample-java: java
	$(PYTHON) Script/generate_sample.py java

sample-all: sample-cpp sample-csharp sample-java


csharp-clean: Dev/bin/Altseed.dll
	rm Dev/bin/Altseed.dll

cpp-clean: Dev/cmake/Makefile Dev/lib/libAltseed.a
	cd Dev/cmake; $(MAKE) clean
	rm Dev/lib/libAltseed.a

java-clean: Dev/asd_java/asd/AltseedObject.java Dev/bin/Altseed.jar
	rm Dev/bin/Altseed.jar
	rm -rf Dev/asd_java/asd

clean: csharp-clean cpp-clean java-clean
