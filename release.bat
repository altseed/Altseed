echo compile
@Set Path=C:\Program Files (x86)\MSBuild\12.0\Bin;%PATH%
msbuild Dev/test_cpp/msvc/test_cpp.vcxproj /p:configuration=Debug
msbuild Dev/test_cpp/msvc/test_cpp.vcxproj /p:configuration=Release

msbuild Dev/test_cs.sln /p:configuration=Release

copy Dev\bin\ace_core.Debug.dll Product\bin\
copy Dev\bin\ace_core.dll Product\bin\
copy Dev\bin\ace_cs.dll Product\bin\

echo CPlusPlus
copy Dev\bin\ace_core.Debug.dll Product\cpp\
copy Dev\bin\ace_core.dll Product\cpp\
copy Dev\include\ace.h Product\cpp\
copy Dev\lib\ace_engine.Debug.lib Product\cpp\
copy Dev\lib\ace_engine.Release.lib Product\cpp\

echo CPlusPlusSample
copy Dev\include\ace.h Product\cpp_sample\
copy Dev\lib\ace_engine.Debug.lib Product\cpp_sample\
copy Dev\lib\ace_engine.Release.lib Product\cpp_sample\

echo CSharp
copy Dev\bin\ace_cs.dll Product\cs\
copy Dev\bin\ace_cs.XML Product\cs\
copy Dev\bin\ace_core.dll Product\cs\

echo CSharpSample
copy Dev\bin\ace_cs.dll Product\cs_sample\
copy Dev\bin\ace_cs.XML Product\cs_sample\

pause