#pragma once
#include <vector>
#include <ft2build.h>
#include "Color.h"
#include FT_GLYPH_H
#include FT_OUTLINE_H

namespace FontGenerator
{
	struct Span
	{
	public:
		Span();
		Span(int _x, int _y, int _width, int _coverage);

		int x, y, width, coverage;
	};

	typedef std::vector<Span> Spans;

	void RasterCallback(
		const int y,
		const int count,
		const FT_Span * const spans,
		void * const user);

	void RenderSpans(FT_Library &library, FT_Outline * const outline, Spans *spans);
}