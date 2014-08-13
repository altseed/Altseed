#include "Glyph.h"
#include "Span.h"
#include <ace.common.Base.h>
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_STROKER_H

using namespace std;

namespace FontGenerator
{

	Glyph::Glyph(FT_Library& library, FT_OutlineGlyph& glyph)
		: m_color(Color()), m_library(&library), m_charactor(L'\0')
	{
		FT_Glyph g;
		FT_Glyph_Copy(&glyph->root, &g);
		m_glyph = reinterpret_cast<FT_OutlineGlyph>(g);
	}

	Glyph::Glyph(FT_Library& library, FT_Face& face, wchar_t ch)
		: m_color(Color())
		, m_library(&library)
		, m_face(face)
		, m_charactor(ch)
	{
		auto index = FT_Get_Char_Index(face, ch);
		FT_Load_Glyph(face, index, FT_LOAD_NO_BITMAP);

		FT_Glyph g;
		FT_Get_Glyph(face->glyph, &g);
		
		ACE_ASSERT(g->format == FT_GLYPH_FORMAT_OUTLINE, "ƒOƒŠƒt‚Ì¶¬‚É¸”s");

		m_glyph = reinterpret_cast<FT_OutlineGlyph>(g);
	}

	Glyph::~Glyph()
	{
		FT_Done_Glyph(&m_glyph->root);
	}

	void Glyph::SetColor(Color color)
	{
		m_color = color;
	}

	int Glyph::GetAdvance() const
	{
		return m_glyph->root.advance.x >> 16;
	}

	wchar_t Glyph::GetCharactor() const
	{
		return m_charactor;
	}

	BorderSetting::Ptr Glyph::GetBorderSetting() const
	{
		return m_border;
	}

	void Glyph::SetBorderSetting(BorderSetting::Ptr value)
	{
		m_border = value;
	}

	void Glyph::Draw(int32_t* buffer, int width, int height, int x, int y)
	{
		Spans spans;
		RenderSpans(*m_library, &m_glyph->outline, &spans);

		for (auto& s : spans)
		{
			int yIndex = y - s.y;
			if (yIndex < 0 || yIndex >= height) continue;
			for (int w = 0; w < s.width; ++w)
			{
				int xIndex = x + s.x + w;
				if (xIndex < 0) continue;
				if (xIndex >= width) break;

				auto c = m_color;
				c.a = (int)(c.a * s.coverage / 255.0f);

				auto& pixel = buffer[yIndex * width + xIndex];
				pixel = Color(pixel).Blend(c).GetInt();
			}
		}
	}

	RasterizedGlyph::Ptr Glyph::Rasterize()
	{
		Spans spans;
		RenderSpans(*m_library, &m_glyph->outline, &spans);

		int width = GetAdvance();
		int height = m_face->size->metrics.height >> 6;
		int baselineY = m_face->size->metrics.ascender >> 6;
		return RasterizedGlyph::FromSpans(spans, width, height, baselineY);
	}
}