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

				Color color(255, 0, 0, s.coverage);
				int index = yIndex * width + xIndex;
				result->buffer[index] = result->buffer[index].Blend(color);
			}
		}

		return result;
	}

	RasterizedGlyph::Ptr RasterizedGlyph::FromSpansWithOutline(
		Spans spans,
		int width,
		int height,
		int baselineY,
		int outline,
		int msaa)
	{
		auto result = make_shared<RasterizedGlyph>(width + outline * 2, height + outline * 2);

		vector<Color> temp((width + outline * 2) * (height + outline * 2) * msaa * msaa);
		auto width_msaa = (width + outline * 2) * msaa;
		auto height_msaa = (height + outline * 2) * msaa;
		auto outline_msaa = outline * msaa;

		// ägëÂÉRÉsÅ[
		for (auto& s : spans)
		{
			int yIndex = baselineY - s.y;
			if (yIndex < 0 || yIndex >= height) continue;

			for (int w = 0; w < s.width; w++)
			{
				int xIndex = s.x + w;
				if (xIndex < 0) continue;
				if (xIndex >= width) break;

				Color color(255, 0, 0, s.coverage);

				int index = ((yIndex + outline) * msaa) * ((width + outline * 2) * msaa) + (xIndex + outline) * msaa;

				for (auto y_ = 0; y_ < msaa; y_++)
				{
					for (auto x_ = 0; x_ < msaa; x_++)
					{
						auto index_ = index + x_ + y_ * ((width + outline * 2) * msaa);
						temp[index_] = color;
					}
				}
			}
		}

		// òg
		for (auto y = 0; y < height_msaa; y++)
		{
			for (auto x = 0; x < width_msaa; x++)
			{
				if (temp[x + width_msaa * y].a > 0) continue;
				for (auto y_ = y - outline_msaa; y_ <= y + outline_msaa; y_++)
				{
					for (auto x_ = x - outline_msaa; x_ <= x + outline_msaa; x_++)
					{
						if (x_ < 0) continue;
						if (x_ >= width_msaa) continue;
						if (y_ < 0) continue;
						if (y_ >= height_msaa) continue;

						if (temp[x_ + width_msaa * y_].r == 255)
						{
							temp[x + width_msaa * y].a = 255;
							temp[x + width_msaa * y].g = 255;
						}
					}
				}
			}
		}

		// èkè¨
		for (auto y = 0; y < height + outline * 2; y++)
		{
			for (auto x = 0; x < width + outline * 2; x++)
			{
				int r = 0;
				int g = 0;
				int b = 0;
				int a = 0;

				for (auto y_ = y * msaa; y_ < y * msaa + msaa; y_++)
				{
					for (auto x_ = x; x_ < x * msaa + msaa; x_++)
					{
						r += temp[x_ + width_msaa * y_].r;
						g += temp[x_ + width_msaa * y_].g;
						b += temp[x_ + width_msaa * y_].b;
						a += temp[x_ + width_msaa * y_].a;
					}
				}

				r /= (msaa);
				g /= (msaa);
				b /= (msaa);
				a /= (msaa);

				int index = x + y * (width + outline * 2);
				result->buffer[index].r = r;
				result->buffer[index].g = g;
				result->buffer[index].b = b;
				result->buffer[index].a = a;
			}
		}

		return result;
	}

	RasterizedGlyph::Ptr RasterizedGlyph::PaintColor(Color color, Color outlineColor)
	{
		auto result = make_shared<RasterizedGlyph>(width, height);

		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < width; x++)
			{
				int index = y * width + x;

				if (buffer[index].a == 0) continue;

				auto a = buffer[index].a;
				if (buffer[index].r > 0)
				{
					result->buffer[index] = color;
					result->buffer[index].a = a;
				}

				if (buffer[index].g == 255)
				{
					result->buffer[index] = outlineColor;
					result->buffer[index].a = a;
				}
			}
		}

		return result;
	}
}