
#include <iostream>
#include <memory>

#include "../FontGenerator/Generator.h"
#include "../FontGenerator/Rendering/Color.h"

#pragma comment(lib,"FontGenerator.lib")

#if _DEBUG
#pragma comment(lib,"freetype253MT_D.lib")
#pragma comment(lib,"libpng16.Debug.lib")
#pragma comment(lib,"zlib.Debug.lib")
#else
#pragma comment(lib,"freetype253MT.lib")
#pragma comment(lib,"libpng16.Release.lib")
#pragma comment(lib,"zlib.Release.lib")
#endif

using namespace std;
using namespace FontGenerator;

int main()
{
	Generator gen;

	try
	{
		SettingForRendering setting;
		setting.SetFontSize(32);
		setting.SetFontColor(Color(255, 255, 255, 255));
		setting.SetBold(make_shared<BoldSetting>(1));
		setting.SetBorder(make_shared<BorderSetting>(4, Color(0, 0, 0, 255)));

		gen.GenerateFontFile(
			L"C:/Windows/Fonts/AzukiP.ttf",
			L"test.txt",
			L"result/test",
			setting);
	}
	catch (const char* err)
	{
		std::cout << err << std::endl;
	}


	system("pause");
	return 0;
}
