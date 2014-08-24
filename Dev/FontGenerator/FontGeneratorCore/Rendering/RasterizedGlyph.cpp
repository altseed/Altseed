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

		auto width_msaa = (width + outline * 2) * msaa;
		auto height_msaa = (height + outline * 2) * msaa;

		vector<Color> temp(width_msaa*height_msaa);
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

				int index = ((yIndex + outline) * msaa) * width_msaa + (xIndex + outline) * msaa;

				for (auto y_ = 0; y_ < msaa; y_++)
				{
					for (auto x_ = 0; x_ < msaa; x_++)
					{
						auto index_ = index + x_ + y_ * width_msaa;
						temp[index_] = temp[index_].Blend(color);
					}
				}
			}
		}

		// òg
		for (auto y = 0; y < height_msaa; y++)
		{
			for (auto x = 0; x < width_msaa; x++)
			{
				auto area = outline_msaa;
				if (msaa >= 2) area -= msaa / 4;

				auto br = false;

				for (auto y_ = y - area; y_ <= y + area; y_++)
				{
					for (auto x_ = x - area; x_ <= x + area; x_++)
					{
						if (x_ < 0) continue;
						if (x_ >= width_msaa) continue;
						if (y_ < 0) continue;
						if (y_ >= height_msaa) continue;

						if (temp[x_ + width_msaa * y_].a > 128)
						{
							auto a_ = temp[x_ + width_msaa * y_].a;
							
							if (temp[x + width_msaa * y].g < a_)
							{
								temp[x + width_msaa * y].g = a_;
							}
							
							if (temp[x + width_msaa * y].g == 255) br = true;
						}
						if (br) break;
					}
					if (br) break;
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
					for (auto x_ = x * msaa; x_ < x * msaa + msaa; x_++)
					{
						r += temp[x_ + width_msaa * y_].r;
						g += temp[x_ + width_msaa * y_].g;
						b += temp[x_ + width_msaa * y_].b;
						a += temp[x_ + width_msaa * y_].a;
					}
				}

				r /= (msaa * msaa);
				g /= (msaa * msaa);
				b /= (msaa * msaa);
				a /= (msaa * msaa);

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

				auto a = buffer[index].a;

				if (buffer[index].r > 0 && buffer[index].g > 0)
				{
					auto a_outline = buffer[index].g;
					auto color_in = color;
					color_in.a = a;
					auto color_out = outlineColor;
					color_out.a = a_outline;

					result->buffer[index] = color_out;
					result->buffer[index] = result->buffer[index].Blend(color_in);
				}
				else if(buffer[index].r > 0)
				{
					result->buffer[index] = color;
					result->buffer[index].a = a;
				}
				else if(buffer[index].g > 0)
				{
					auto a_outline = buffer[index].g;
					result->buffer[index] = outlineColor;
					result->buffer[index].a = a_outline;
				}
			}
		}

		return result;
	}
}