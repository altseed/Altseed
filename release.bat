
SET RDIR=ACE140308

echo compile
@Set Path=C:\Program Files (x86)\MSBuild\12.0\Bin;%PATH%
msbuild Dev/unitTest_Engine_cpp.sln /p:configuration=Debug
msbuild Dev/unitTest_Engine_cpp.sln /p:configuration=Release
msbuild Dev/test_cs.sln /p:configuration=Release

echo GenDirectories
mkdir %RDIR%\
mkdir %RDIR%\cpp\
mkdir %RDIR%\cpp\Debug\
mkdir %RDIR%\cpp\Release\
mkdir %RDIR%\cs\

mkdir %RDIR%\Sample\
mkdir %RDIR%\Sample\sample_cpp\
mkdir %RDIR%\Sample\sample_cs\
mkdir %RDIR%\Sample\bin\
mkdir %RDIR%\Sample\cpp\
mkdir %RDIR%\Sample\cpp\include\
mkdir %RDIR%\Sample\cpp\lib\
mkdir %RDIR%\Sample\cpp\lib\Debug\
mkdir %RDIR%\Sample\cpp\lib\Release\

echo CPlusPlus
copy Dev\bin\ace_core.Debug.dll %RDIR%\cpp\
copy Dev\bin\ace_core.dll %RDIR%\cpp\
copy Dev\include\ace.h %RDIR%\cpp\
copy Dev\lib\Debug\ace_engine.lib %RDIR%\cpp\Debug\
copy Dev\lib\Release\ace_engine.lib %RDIR%\cpp\Release\

echo CSharp
copy Dev\bin\ace_cs.dll %RDIR%\cs\
copy Dev\bin\ace_cs.XML %RDIR%\cs\
copy Dev\bin\ace_core.dll %RDIR%\cs\

echo Sample

robocopy Sample\bin\ %RDIR%\Sample\bin\ *.ogg *.wav /mir /S
robocopy Sample\ %RDIR%\Sample\ *.sln /mir /S
robocopy Sample\sample_cpp %RDIR%\Sample\sample_cpp\ *.h *.cpp *.filters *.vcxproj /mir /S
robocopy Sample\sample_cs %RDIR%\Sample\sample_cs\ *.cs *.csproj /mir /S

copy Dev\bin\ace_core.Debug.dll %RDIR%\Sample\bin\
copy Dev\bin\ace_core.dll %RDIR%\Sample\bin\
copy Dev\bin\ace_cs.dll %RDIR%\Sample\bin\

echo CPlusPlus
copy Dev\include\ace.h %RDIR%\Sample\cpp\include\
copy Dev\lib\Debug\ace_engine.lib %RDIR%\Sample\cpp\lib\Debug\
copy Dev\lib\Release\ace_engine.lib %RDIR%\Sample\cpp\lib\Release\

echo CSharp
copy Dev\bin\ace_cs.dll %RDIR%\Sample\sample_cs\
copy Dev\bin\ace_cs.XML %RDIR%\Sample\sample_cs\

pause