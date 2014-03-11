
import aceutils

aceutils.cd(r"../")

aceutils.rmdir(r"gtest_bin")
aceutils.rmdir(r"gtest-1.7.0")

aceutils.wget(r"http://googletest.googlecode.com/files/gtest-1.7.0.zip")

aceutils.unzip(r"gtest-1.7.0.zip")

aceutils.mkdir(r"gtest_bin")

aceutils.cd(r"gtest_bin")
aceutils.call(r'cmake -G "Visual Studio 12" -D BUILD_SHARED_LIBS:BOOL=OFF ../gtest-1.7.0/')
aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" gtest.sln /p:configuration=Debug')
aceutils.call(r'"C:\Program Files (x86)\MSBuild\12.0\Bin\msbuild" gtest.sln /p:configuration=Release')

aceutils.copy(r'Debug/gtest.lib', r'../Dev/lib/gtestd.lib')
aceutils.copy(r'Debug/gtest_main.lib', r'../Dev/lib/gtest_maind.lib')

aceutils.copy(r'Release/gtest.lib', r'../Dev/lib/gtest.lib')
aceutils.copy(r'Release/gtest_main.lib', r'../Dev/lib/gtest_main.lib')

aceutils.copytree(r'../gtest-1.7.0/include/gtest', r'../Dev/include/gtest/')








