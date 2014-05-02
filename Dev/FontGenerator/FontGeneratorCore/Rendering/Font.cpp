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
	}

	Font::~Font()
	{
		FT_Done_Face(m_face);
		FT_Done_FreeType(m_library);
	}

	vector<Glyph::Ptr> Font::GetGlyphs(vector<achar>& charactors)
	{
		vector<Glyph::Ptr> glyphs;
		for (auto& c : charactors)
		{
			auto g = make_shared<Glyph>(m_library, m_face, c);
			glyphs.push_back(g);
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
		return m_face->size->metrics.height >> 6;
	}
}