SET RDIR=Sample

echo compile
@Set Path=C:\Program Files (x86)\MSBuild\12.0\Bin;%PATH%
msbuild Dev/unitTest_Engine_cpp.sln /p:configuration=Debug
msbuild Dev/unitTest_Engine_cpp.sln /p:configuration=Release

msbuild Dev/test_cs.sln /p:configuration=Release

mkdir %RDIR%\bin\
mkdir %RDIR%\cpp\
mkdir %RDIR%\cpp\include\
mkdir %RDIR%\cpp\lib\
mkdir %RDIR%\cs\

copy Dev\bin\ace_core.Debug.dll %RDIR%\bin\
copy Dev\bin\ace_core.dll %RDIR%\bin\
copy Dev\bin\ace_cs.dll %RDIR%\bin\

echo CPlusPlusSample
copy Dev\include\ace.h %RDIR%\cpp\include\
copy Dev\lib\ace_engine.Debug.lib %RDIR%\cpp\lib\
copy Dev\lib\ace_engine.Release.lib %RDIR%\cpp\lib\

echo CSharpSample
copy Dev\bin\ace_cs.dll %RDIR%\sample_cs\
copy Dev\bin\ace_cs.XML %RDIR%\sample_cs\

pause