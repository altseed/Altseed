#include "RasterizedGlyph.h"

using namespace std;

namespace FontGenerator
{
	RasterizedGlyph::RasterizedGlyph(int width, int height)
		: buffer(vector<Color>(width * height))
		, width(width)
		, height(height)
	{
	}

	RasterizedGlyph::Ptr RasterizedGlyph::FromSpans(
		Spans spans,
		int width,
		int height,
		int baselineY)
	{
		auto result = make_shared<RasterizedGlyph>(width, height);

		for (auto& s : spans)
		{
			int yIndex = baselineY - s.y;
			if (yIndex < 0 || yIndex >= height) continue;

			for (int w = 0; w < s.width; w++)
			{
				int xIndex = s.x + w;
				if (xIndex < 0) continue;
				if (xIndex >= width) break;

				Color color(0, 0, 0, s.coverage);
				int index = yIndex * width + xIndex;
				result->buffer[index] = result->buffer[index].Blend(color);
			}
		}

		return result;
	}

	RasterizedGlyph::Ptr RasterizedGlyph::PaintColor(Color color)
	{
		auto result = make_shared<RasterizedGlyph>(width, height);

		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < width; x++)
			{
				int index = y * width + x;

				auto c = color;
				c.a = buffer[index].a;

				result->buffer[index] = c;
			}
		}

		return result;
	}
}