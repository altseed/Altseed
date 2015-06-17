
#include <iostream>
#include <memory>

#include "../FontGeneratorCore/Generator.h"
#include "../FontGeneratorCore/Rendering/Color.h"
#include "../../asd_cpp/common/Graphics/Font/asd.AffLoader.h"

#pragma comment(lib,"asd_common.lib")
#pragma comment(lib,"FontGeneratorCore.lib")

#if _DEBUG
#pragma comment(lib,"x86/Debug/freetype.lib")
#pragma comment(lib,"libpng16.Debug.lib")
#pragma comment(lib,"zlib.Debug.lib")
#else
#pragma comment(lib,"x86/Release/freetype.lib")
#pragma comment(lib,"libpng16.Release.lib")
#pragma comment(lib,"zlib.Release.lib")
#endif

using namespace std;
using namespace FontGenerator;

void Test()
{
	Generator gen;
	auto chars = gen.GetCharactors(asd::ToAString("test2.txt"));
}

int main()
{
	//Test();
	//return 0;

	Generator gen;

	try
	{
		SettingForRendering setting;
		setting.SetFontSize(32);
		setting.SetFontColor(Color(255, 128, 0, 255));
		setting.SetBorder(make_shared<BorderSetting>(1, Color(0, 0, 0, 255), 1));
		
		gen.SetSetting(setting);
		gen.SetSheetName(L"result/Nac1220_1");

		gen.GenerateFontFile(
			L"C:/Windows/Fonts/FlopDesignFONT.otf",
			//L"C:/Windows/Fonts/KozGoPr6N-Regular.otf",
			//L"C:/Windows/Fonts/meiryo.ttc",
			L"test.txt");
	}
	catch (const char* err)
	{
		std::cout << err << std::endl;
	}


	system("pause");
	return 0;
}
