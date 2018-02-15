#include "Font.h"
#include FT_OUTLINE_H

using namespace std;
using namespace asd;

namespace FontGenerator
{
	Font::Font(astring fontPath)
	{
		auto error = FT_Init_FreeType(&m_library);
		ACE_ASSERT(!error, "FreeTypeの初期化に失敗しました");

		
#ifdef _WIN32
		FILE* fp = nullptr;

		_wfopen_s(&fp, (const wchar_t*)fontPath.c_str(), L"rb");
		if (fp != nullptr)
		{
			fseek(fp, 0, SEEK_END);
			auto size = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			
			fontBuffer.resize(size);

			fread(fontBuffer.data(), 1, size, fp);
			fclose(fp);

			error = FT_New_Memory_Face(
				m_library,
				fontBuffer.data(),
				size,
				0,
				&m_face);
			ACE_ASSERT(!error, "Faceの初期化に失敗しました");
		}
		else
		{
			ACE_ASSERT(fp != nullptr, "Faceの初期化に失敗しました");
		}
#else
		auto path = ToUtf8String(fontPath.c_str());
		error = FT_New_Face(m_library, path.c_str(), 0, &m_face);
		ACE_ASSERT(!error, "Faceの初期化に失敗しました");
#endif
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
			auto index = FT_Get_Char_Index(m_face, c);
			FT_Load_Glyph(m_face, index, FT_LOAD_NO_BITMAP);

			FT_Glyph g;
			FT_Get_Glyph(m_face->glyph, &g);

			ACE_ASSERT(g->format == FT_GLYPH_FORMAT_OUTLINE, "グリフの生成に失敗");

			auto og = reinterpret_cast<FT_OutlineGlyph>(g);
			glyphs.push_back(make_shared<Glyph>(*this, c, og));
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
		//auto error = FT_Set_Char_Size(m_face, value << 6, value << 6, 96, 96);
		auto error = FT_Set_Pixel_Sizes(m_face, value, value);
		ACE_ASSERT(!error, "フォントサイズの設定に失敗しました")
	}

	int Font::GetFontHeight() const
	{
		return m_face->size->metrics.height >> 6;
	}

	int Font::GetAscender() const
	{
		return m_face->size->metrics.ascender >> 6;
	}

	int Font::GetDescender() const
	{
		return m_face->size->metrics.descender >> 6;
	}

	FT_Library Font::GetLibrary() const
	{
		return m_library;
	}
}