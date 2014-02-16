#include <fstream>
#include <freetype/ft2build.h>
#include FT_FREETYPE_H
#include "FontGenerator.h"
#include "AffHeader.h"
#include "PngGenerator.h"

#pragma comment(linker, "/nodefaultlib:libcmtd.lib")
#pragma comment(linker, "/nodefaultlib:LIBCMT.lib")

using namespace std;
using namespace ace;

namespace FontGenerator
{
	void FontGenerator::GenerateFontFile(
		wstring fontPath,
		int fontSize,
		wstring textPath,
		wstring sheetName)
	{
		ofstream file(sheetName + L".aff");

		auto charactors = GetCharactors(ToAString(textPath.c_str()));

		PngGenerator png;
		png.SetFontSize(fontSize);
		png.SetSheetName(sheetName);
		png.SetSheetSize(256);
		auto result = png.Generate(ToAString(fontPath.c_str()), charactors);

		AffHeader header(sheetName);
		header.SetFontSize(fontSize);
		header.SetFontCount(result.fonts.size());
		header.SetSheetCount(result.sheetCount);

		header.WriteOut(file);
	}

	vector<achar> FontGenerator::GetCharactors(astring textPath)
	{
		vector<achar> result;

		FILE* file;
		_wfopen_s(&file, textPath.c_str(), L"r");

		while (!feof(file))
		{
			achar c;
			fread(&c, sizeof(achar), 1, file);
			result.push_back(c);
		}

		fclose(file);

		return result;
	}
}