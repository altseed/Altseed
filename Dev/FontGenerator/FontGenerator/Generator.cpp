#include <fstream>
#include <freetype/ft2build.h>
#include FT_FREETYPE_H
#include "Generator.h"
#include "AffHeader.h"
#include "PngGenerator.h"

#pragma comment(linker, "/nodefaultlib:libcmtd.lib")
#pragma comment(linker, "/nodefaultlib:LIBCMT.lib")

using namespace std;
using namespace ace;

namespace FontGenerator
{
	void Generator::GenerateFontFile(
		wstring fontPath,
		wstring textPath,
		wstring sheetName,
		SettingForRendering setting)
	{
		ofstream file(sheetName + L".aff");

		auto charactors = GetCharactors(ToAString(textPath.c_str()));

		PngGenerator png;
		png.SetSetting(setting);
		png.SetSheetName(sheetName);
		png.SetSheetSize(256);
		auto result = png.Generate(ToAString(fontPath.c_str()), charactors);

		AffHeader header(sheetName);
		header.SetFontSize(setting.GetFontSize());
		header.SetFontCount(result.fonts.size());
		header.SetSheetCount(result.sheetCount);

		header.WriteOut(file);
	}

	// UTF-16Ç™ëŒè€
	vector<achar> Generator::GetCharactors(astring textPath)
	{
		vector<achar> result;

		FILE* file;
		_wfopen_s(&file, textPath.c_str(), L"r");

		achar bom;
		fread(&bom, sizeof(achar), 1, file);

		while (!feof(file))
		{
			achar c;
			if (fread(&c, sizeof(achar), 1, file) == 1)
			{
				result.push_back(c);
			}
		}

		fclose(file);

		return result;
	}
}