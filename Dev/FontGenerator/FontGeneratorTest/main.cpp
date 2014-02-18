#include <iostream>
#include "../FontGenerator/FontGenerator.h"

int main()
{
	FontGenerator::FontGenerator gen;

	try
	{
		gen.GenerateFontFile(
			L"C:/Windows/Fonts/azuki.ttf",
			24,
			L"test.txt",
			L"test");
	}
	catch (const char* err)
	{
		std::cout << err << std::endl;
	}

	system("pause");
	return 0;
}