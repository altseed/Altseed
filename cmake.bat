echo cmake
cd Dev
rmdir /S /Q cmake
mkdir cmake
cd cmake
cmake -G "Visual Studio 12" -D BUILD_SHARED_LIBS:BOOL=OFF -D USE_MSVC_RUNTIME_LIBRARY_DLL:BOOL=OFF ../

pause
