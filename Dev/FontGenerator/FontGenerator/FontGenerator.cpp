#include <fstream>
#include <freetype/ft2build.h>
#include FT_FREETYPE_H
#include "FontGenerator.h"
#include "AffHeader.h"

using namespace std;

namespace FontGenerator
{
	void FontGenerator::GenerateFontFile(
		wstring fontPath,
		int fontSize,
		wstring textPath,
		wstring sheetName)
	{
		ofstream file(sheetName + L".aff");

		AffHeader header(fontSize, sheetName);
		auto headerBytes = header.GetBytes();
		file.write(headerBytes.data(), headerBytes.size());
	}
}