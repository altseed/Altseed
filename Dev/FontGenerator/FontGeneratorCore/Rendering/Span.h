#pragma once
#include <vector>
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_STROKER_H

namespace FontGenerator
{
	struct Span
	{
		Span() {}
		Span(int _x, int _y, int _width, int _coverage)
			: x(_x), y(_y), width(_width), coverage(_coverage)
		{
		}

		int x, y, width, coverage;
	};

	typedef std::vector<Span> Spans;

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

	void RenderSpans(
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
}