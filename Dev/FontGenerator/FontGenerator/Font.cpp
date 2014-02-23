#include "Font.h"
#include FT_OUTLINE_H

using namespace std;
using namespace ace;

namespace FontGenerator
{
	Font::Font(astring fontPath)
	{
		auto error = FT_Init_FreeType(&m_library);
		if (error)
		{
			throw "FreeTypeの初期化に失敗しました";
		}

		auto path = ToUtf8String(fontPath.c_str());
		error = FT_New_Face(m_library, path.c_str(), 0, &m_face);
		if (error)
		{
			throw "Faceの初期化に失敗しました";
		}

		SetFontSize(16);

		FT_Outline_Embolden();
	}

	Font::~Font()
	{
		for (auto& g : m_cashForRelease)
		{
			FT_Done_Glyph(g);
		}
		
		FT_Done_Face(m_face);

		FT_Done_FreeType(m_library);
	}

	vector<FT_BitmapGlyph> Font::GetGlyphs(vector<achar>& charactors)
	{
		vector<FT_BitmapGlyph> glyphs;
		for (auto& c : charactors)
		{
			FT_Glyph glyph;
			FT_BitmapGlyph bitmap;

			auto error = FT_Load_Char(m_face, c, FT_LOAD_RENDER);
			if (error)
			{
				throw "グリフの生成に失敗しました。";
			}

			FT_Get_Glyph(m_face->glyph, &glyph);
			m_cashForRelease.push_back(glyph);

			bitmap = (FT_BitmapGlyph)glyph;
			glyphs.push_back(bitmap);
		}
		return glyphs;
	}

	int Font::GetFontSize() const
	{
		return m_fontSize;
	}

	void Font::SetFontSize(int value)
	{
		m_fontSize = value;
		//auto error = FT_Set_Char_Size(m_face, 0, value * 64, 96, 96);
		auto error = FT_Set_Pixel_Sizes(m_face, 0, value);
		if (error)
		{
			throw "フォントサイズの設定に失敗しました";
		}
	}

	int Font::GetFontHeight() const
	{
		return m_face->size->metrics.height;
	}
}