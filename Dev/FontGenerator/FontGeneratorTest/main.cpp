#include <iostream>
#include <memory>
#include "../FontGenerator/Generator.h"
#include "../FontGenerator/Rendering/Color.h"

using namespace std;
using namespace FontGenerator;

int main()
{
	Generator gen;

	try
	{
		SettingForRendering setting;
		setting.SetFontSize(32);
		setting.SetFontColor(Color(255, 0, 0, 255));
		setting.SetBorder(make_shared<BorderSetting>(2, Color(0, 0, 0, 128)));

		gen.GenerateFontFile(
			L"C:/Windows/Fonts/azuki.ttf",
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