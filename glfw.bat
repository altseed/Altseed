echo cmake
rmdir /S /Q glfw_bin
mkdir glfw_bin
cd glfw_bin
cmake -G "Visual Studio 12" -D BUILD_SHARED_LIBS:BOOL=OFF -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ../Dev/amcr-glfw/

echo compile
@Set Path=C:\Program Files (x86)\MSBuild\12.0\Bin;%PATH%
msbuild src/glfw.vcxproj /p:configuration=Debug
msbuild src/glfw.vcxproj /p:configuration=Release

echo move
mkdir ..\Dev\lib\Debug
mkdir ..\Dev\lib\Release
move src\Debug\glfw3.lib ../Dev/lib/Debug/
move src\Release\glfw3.lib ../Dev/lib/Release/

cd ..
mkdir Dev\include\GLFW
copy Dev\amcr-glfw\include\GLFW\glfw3.h Dev\include\GLFW\
copy Dev\amcr-glfw\include\GLFW\glfw3native.h Dev\include\GLFW\

pause