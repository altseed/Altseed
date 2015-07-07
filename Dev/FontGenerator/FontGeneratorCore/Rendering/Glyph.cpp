#include "Glyph.h"
#include "Span.h"
#include "Font.h"
#include <asd.common.Base.h>
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_STROKER_H

using namespace std;

namespace FontGenerator
{
	Glyph::Glyph(Font& font, asd::achar charctor, FT_OutlineGlyph glyph)
		: font(font)
		, m_charactor(charctor)
		, m_glyph(glyph)
	{
	}

	Glyph::~Glyph()
	{
		FT_Done_Glyph(&m_glyph->root);
	}

	int Glyph::GetAdvance() const
	{
		return m_glyph->root.advance.x >> 16;
	}

	wchar_t Glyph::GetCharactor() const
	{
		return m_charactor;
	}

	RasterizedGlyph::Ptr Glyph::Rasterize()
	{
		Spans spans;
		FT_Library lib = font.GetLibrary();
		RenderSpans(lib, &m_glyph->outline, &spans);

		int width = GetAdvance();
		int height = font.GetFontHeight();
		int baselineY = font.GetAscender();
		return RasterizedGlyph::FromSpans(spans, width, height, baselineY);
	}

	RasterizedGlyph::Ptr Glyph::RasterizeWithOutline(int32_t outline)
	{
		Spans spans;
		FT_Library lib = font.GetLibrary();
		RenderSpans(lib, &m_glyph->outline, &spans);

		int width = GetAdvance();
		int height = font.GetFontHeight();
		int baselineY = font.GetAscender();
		return RasterizedGlyph::FromSpansWithOutline(spans, width, height, baselineY, outline);
	}
}