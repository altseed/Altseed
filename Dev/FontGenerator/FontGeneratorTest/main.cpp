
#include <iostream>
#include <memory>

#include "../FontGeneratorCore/Generator.h"
#include "../FontGeneratorCore/Rendering/Color.h"
#include "../../ace_cpp/common/Graphics/Font/ace.AffLoader.h"

#pragma comment(lib,"ace_common.lib")
#pragma comment(lib,"FontGeneratorCore.lib")

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

void Test()
{
	Generator gen;
	auto chars = gen.GetCharactors(ace::ToAString("test2.txt"));
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
