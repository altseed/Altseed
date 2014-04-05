#include <fstream>
#include <freetype/ft2build.h>
#include FT_FREETYPE_H
#include "Generator.h"
#include "AffHeader.h"
#include <Utility/ace.BinaryWriter.h>
#include <Utility/ace.BinaryReader.h>

#pragma comment(linker, "/nodefaultlib:libcmtd.lib")
#pragma comment(linker, "/nodefaultlib:LIBCMT.lib")

using namespace std;
using namespace ace;

namespace FontGenerator
{
	void Generator::WriteIndexTableOut(ResultOfGeneratingPng& result, ostream& stream)
	{
		vector<int16_t> indexes(INDEX_MAX, 0);

		for (int i = 0; i < result.fonts.size(); ++i)
		{
			indexes[result.fonts[i].charactor] = (int16_t)i;
		}

		BinaryWriter writer;
		for (auto& index : indexes)
		{
			writer.Push(index);
		}

		writer.WriteOut(stream);
	}

	void Generator::WriteFontBalkOut(ResultOfGeneratingPng& result, std::ostream& stream)
	{
		BinaryWriter writer;
		for (auto& f : result.fonts)
		{
			writer.Push(f.sheetNumber);
			writer.Push(f.x);
			writer.Push(f.y);
			writer.Push(f.width);
			writer.Push(f.height);
		}
		writer.WriteOut(stream);
	}

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

		WriteIndexTableOut(result, file);

		WriteFontBalkOut(result, file);
	}

	// UTF-16‚ª‘ÎÛ
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
				if (find(result.begin(), result.end(), c) == result.end())
				{
					result.push_back(c);
				}
			}
		}

		fclose(file);

		return result;
	}
}