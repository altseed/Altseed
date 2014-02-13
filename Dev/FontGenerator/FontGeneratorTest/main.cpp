#include "../FontGenerator/FontGenerator.h"
#pragma comment(lib, "../Debug/FontGenerator.lib")

int main()
{
	FontGenerator::FontGenerator gen;

	gen.GenerateFontFile(
		L"C:/Windows/Fonts/consola.ttf",
		16,
		L"test.txt",
		L"sample");

	return 0;
}