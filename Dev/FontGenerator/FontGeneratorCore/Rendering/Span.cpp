#include "Span.h"

namespace FontGenerator
{
	Span::Span()
		: x(0), y(0), width(0), coverage(0)
	{
	}

	Span::Span(int x, int y, int width, int coverage)
		: x(x), y(y), width(width), coverage(coverage)
	{
	}

	void RasterCallback(
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