rmdir /S /Q gtest_bin
rmdir /S /Q gtest-1.7.0

call wget.bat http://googletest.googlecode.com/files/gtest-1.7.0.zip

unzip gtest-1.7.0.zip

mkdir gtest_bin
cd gtest_bin
cmake -G "Visual Studio 12" -D BUILD_SHARED_LIBS:BOOL=OFF ../gtest-1.7.0/

@Set Path=C:\Program Files (x86)\MSBuild\12.0\Bin;%PATH%
msbuild gtest.sln /p:configuration=Debug
msbuild gtest.sln /p:configuration=Release

copy Debug\gtest.lib ..\Dev\lib\gtestd.lib /Y
copy Debug\gtest_main.lib ..\Dev\lib\gtest_maind.lib /Y

copy Release\gtest.lib ..\Dev\lib\gtest.lib /Y
copy Release\gtest_main.lib ..\Dev\lib\gtest_main.lib /Y

robocopy /E ../gtest-1.7.0/include/gtest ../Dev/include/gtest/
