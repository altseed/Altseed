
#include <iostream>
#include <memory>

#include "../FontGeneratorCore/Generator.h"
#include "../FontGeneratorCore/Utility.h"

#include "../FontGeneratorCore/Rendering/Color.h"

#include "../../asd_cpp/common/Graphics/Font/asd.AffLoader.h"

#include "../../asd_cpp/tool/Font/asd.FontRasterizer.h"

#pragma comment(lib,"asd_common.lib")
#pragma comment(lib,"asd_tool.lib")
#pragma comment(lib,"FontGeneratorCore.lib")

#if _DEBUG
#pragma comment(lib,"x86/Debug/freetype.lib")
#pragma comment(lib,"x86/Debug/libpng16.lib")
#pragma comment(lib,"x86/Debug/zlib.lib")
#else
#pragma comment(lib,"x86/Release/freetype.lib")
#pragma comment(lib,"x86/Release/libpng16.lib")
#pragma comment(lib,"x86/Release/zlib.lib")
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
	{
		asd::FontRasterizer rasterizer;

		auto fp = fopen("C:\\Windows\\Fonts\\meiryo.ttc", "rb");
		fseek(fp, 0, SEEK_END);
		auto size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		std::vector<uint8_t> data;
		data.resize(size);
		fread(data.data(), 1, size, fp);
		fclose(fp);

		if (rasterizer.Initialize(data.data(), data.size(), 14, 0, asd::Color(100, 0, 0, 255), asd::Color(0, 100, 0, 255), 1024))
		{
			rasterizer.AddGlyph(asd::ToAString("a").c_str()[0]);
			rasterizer.AddGlyph(asd::ToAString("b").c_str()[0]);
			rasterizer.AddGlyph(asd::ToAString("c").c_str()[0]);
			rasterizer.AddGlyph(asd::ToAString("d").c_str()[0]);

			SavePNGImage(L"result.png", 1024, 1024, rasterizer.GetImages()[0]->Buffer.data(), false);
		}
		
		return 0;
	}
	
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
