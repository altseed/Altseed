
#include <fstream>
#include "Generator.h"
#include <Graphics/Font/ace.AffHeader.h>
#include <Graphics/Font/ace.AffIndexTable.h>
#include <Graphics/Font/ace.GlyphData.h>
#include <Utility/ace.BinaryWriter.h>
#include <Utility/ace.BinaryReader.h>
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;
using namespace ace;

namespace FontGenerator
{
	Generator::Generator()
		: m_sheetName(ToAString(L"font"))
	{
		m_setting = SettingForRendering();
		m_setting.SetBold(nullptr);
		m_setting.SetBorder(nullptr);
		m_setting.SetFontSize(16);
		m_setting.SetFontColor(Color(0, 0, 0, 255));
	}

	void Generator::GenerateFontFile(
		wstring fontPath,
		wstring textPath)
	{
		auto result = RenderPng(fontPath, textPath);

		BinaryWriter writer;
		PushAff(writer, result);

		ofstream file(m_sheetName + L".aff", ios::out | ios::binary);
		writer.WriteOut(file);
	}

	ResultOfGeneratingPng Generator::RenderPng(wstring fontPath, wstring textPath)
	{
		auto charactors = GetCharactors(ToAString(textPath.c_str()));

		PngGenerator png;
		png.SetSetting(m_setting);
		png.SetSheetName(m_sheetName);
		png.SetSheetSize(256);

		auto result = png.Generate(ToAString(fontPath.c_str()), charactors);
		return result;
	}

	void Generator::PushAff(BinaryWriter& writer, ResultOfGeneratingPng& result)
	{
		AffHeader header(m_sheetName);
		header.SetFontSize(m_setting.GetFontSize());
		header.SetFontCount(result.fonts.size());
		header.SetSheetCount(result.sheetCount);

		AffIndexTable indexTable;
		for (int i = 0; i < result.fonts.size(); ++i)
		{
			indexTable.AppendFontIndex(result.fonts[i].GetCharactor(), (int16_t)i);
		}

		header.Push(writer);
		indexTable.Push(writer);
		for (auto& f : result.fonts)
		{
			f.Push(writer);
		}
	}

	// UTF-16Ç™ëŒè€
	vector<achar> Generator::GetCharactors(astring textPath)
	{
		std::set<achar> charactors;

		// rÇæÇ∆ì«Ç›çûÇﬂÇ»Ç¢ï∂éöÇ™Ç†ÇÈÇÃÇ≈rbÇ≈ì«Ç›çûÇﬁ
		FILE* file = nullptr;
		_wfopen_s(&file, textPath.c_str(), L"rb");

		// BOMèàóù
		achar bom;
		fread(&bom, sizeof(achar), 1, file);
		if (bom != 65279)
		{
			charactors.insert(bom);
		}

		// ï∂éöë„ì¸
		while (!feof(file))
		{
			achar c;
			if (fread(&c, sizeof(achar), 1, file) == 1)
			{
				charactors.insert(c);
			}
		}

		fclose(file);

		// å›ä∑ê´à€éù
		vector<achar> result;
		for (auto c : charactors)
		{
			result.push_back(c);
		}

		return result;
	}

	wstring Generator::GetSheetName() const
	{
		return m_sheetName;
	}

	void Generator::SetSheetName(wstring value)
	{
		m_sheetName = value;
	}

	SettingForRendering Generator::GetSetting() const
	{
		return m_setting;
	}

	void Generator::SetSetting(SettingForRendering value)
	{
		m_setting = value;
	}

}