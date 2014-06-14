
#include <iostream>
#include <memory>

#include "../FontGeneratorCore/Generator.h"
#include "../FontGeneratorCore/Rendering/Color.h"

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
	auto chars = gen.GetCharactors(ace::ToAString("textFile.txt"));
	for (auto& c : chars)
	{
		wprintf(L"%c", c);
	}


}

int main()
{
	Test();

	Generator gen;

	try
	{
		SettingForRendering setting;
		setting.SetFontSize(32);
		setting.SetFontColor(Color(255, 0, 0, 255));
		//setting.SetBold(make_shared<BoldSetting>(1));
		//setting.SetBorder(make_shared<BorderSetting>(4, Color(0, 0, 0, 255)));
		
		gen.SetSetting(setting);
		gen.SetSheetName(L"result/test");

		gen.GenerateFontFile(
			L"C:/Windows/Fonts/Consola.ttf",
			L"test.txt");
	}
	catch (const char* err)
	{
		std::cout << err << std::endl;
	}


	system("pause");
	return 0;
}
