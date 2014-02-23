#include "Glyph.h"
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_STROKER_H

using namespace std;

namespace FontGenerator
{
	Glyph::Glyph(FT_Library& library, FT_OutlineGlyph& glyph)
		: m_color(Color()), m_library(&library)
	{
		FT_Glyph g;
		FT_Glyph_Copy(&glyph->root, &g);
		m_glyph = reinterpret_cast<FT_OutlineGlyph>(g);
	}

	Glyph::Glyph(FT_Library& library, FT_Face& face, wchar_t ch)
		: m_color(Color()), m_library(&library)
	{
		auto index = FT_Get_Char_Index(face, ch);
		FT_Load_Glyph(face, index, FT_LOAD_NO_BITMAP);

		FT_Glyph g;
		FT_Get_Glyph(face->glyph, &g);
		if (g->format == FT_GLYPH_FORMAT_OUTLINE)
		{
			m_glyph = reinterpret_cast<FT_OutlineGlyph>(g);
		}
		else
		{
			throw "ƒOƒŠƒt‚Ì¶¬‚ÉŽ¸”s";
		}
	}

	Glyph::~Glyph()
	{
		FT_Done_Glyph(&m_glyph->root);
	}

	void Glyph::SetColor(Color color)
	{
		m_color = color;
	}

	Glyph::Ptr Glyph::Border(float width, Color color)
	{
		FT_Glyph g;
		FT_Glyph_Copy(&m_glyph->root, &g);

		FT_Stroker stroker;
		FT_Stroker_New(*m_library, &stroker);
		FT_Stroker_Set(
			stroker,
			(int)(width * 64),
			FT_STROKER_LINECAP_ROUND,
			FT_STROKER_LINEJOIN_ROUND,
			0);

		FT_Glyph_StrokeBorder(&g, stroker, 0, 1);
		FT_Stroker_Done(stroker);

		auto o = reinterpret_cast<FT_OutlineGlyph>(g);
		auto result = Glyph::Ptr(new Glyph(*m_library, o));
		result->SetColor(color);

		FT_Done_Glyph(g);

		return result;
	}

	Glyph::Ptr Glyph::Enbolden(float weight)
	{
		FT_Glyph g;
		FT_Glyph_Copy(&m_glyph->root, &g);

		FT_OutlineGlyph o = reinterpret_cast<FT_OutlineGlyph>(g);
		FT_Outline_Embolden(&o->outline, (long)(weight * 64));

		auto result = Glyph::Ptr(new Glyph(*m_library, o));
		result->SetColor(m_color);

		FT_Done_Glyph(g);

		return result;
	}

	static void RasterCallback(
		const int y,
		const int count,
		const FT_Span * const spans,
		void * const user)
	{
		Spans *sptr = (Spans *)user;
		for (int i = 0; i < count; ++i)
			sptr->push_back(Span(spans[i].x, y, spans[i].len, spans[i].coverage));
	}

	static void RenderSpans(
		FT_Library &library,
		FT_Outline * const outline,
		Spans *spans)
	{
		FT_Raster_Params params;
		memset(&params, 0, sizeof(params));
		params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
		params.gray_spans = RasterCallback;
		params.user = spans;

		FT_Outline_Render(library, outline, &params);
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

	int Glyph::GetAdvance() const
	{
		return m_glyph->root.advance.x >> 16;
	}
}