mkdir Product
mkdir Product\include
mkdir Product\bin
mkdir Product\lib
mkdir Product\lib\Debug
mkdir Product\lib\Release

copy Dev\include\ace.h Product\include\ace.h

copy Dev\bin\ace_cs.dll Product\bin\ace_cs.dll
copy Dev\bin\ace_core.dll Product\bin\ace_core.dll

copy Dev\lib\ace_common.Debug.lib Product\lib\ace_common.Debug.lib
copy Dev\lib\ace_common.Release.lib Product\lib\ace_common.Release.lib
copy Dev\lib\ace_engine.Debug.lib Product\lib\ace_engine.Debug.lib
copy Dev\lib\ace_engine.Release.lib Product\lib\ace_engine.Release.lib
copy Dev\lib\Debug\*.lib Product\lib\Debug\
copy Dev\lib\Release\*.lib Product\lib\Release\

xcopy Dev\test_cs\* Product\sample_cs\ /E /EXCLUDE:exclude.txt
xcopy Dev\test_cpp\* Product\sample_cpp\ /E /EXCLUDE:exclude.txt

pause