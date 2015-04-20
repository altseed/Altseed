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

				Color color(0, 0, 255, s.coverage);
				int index = yIndex * width + xIndex;
				result->buffer[index] = color;
			}
		}

		return result;
	}

	RasterizedGlyph::Ptr RasterizedGlyph::FromSpansWithOutline(
		Spans spans,
		int width,
		int height,
		int baselineY,
		int outline)
	{
		auto result = make_shared<RasterizedGlyph>(width + outline * 2, height + outline * 2);

		auto width_o = (width + outline * 2);
		auto height_o = (height + outline * 2);

		// ÉRÉsÅ[
		for (auto& s : spans)
		{
			int yIndex = baselineY - s.y;
			if (yIndex < 0 || yIndex >= height) continue;

			for (int w = 0; w < s.width; w++)
			{
				int xIndex = s.x + w;
				if (xIndex < 0) continue;
				if (xIndex >= width) break;

				Color color(0, 0, 255, s.coverage);

				int index = ((yIndex + outline)) * width_o+ (xIndex + outline);

				result->buffer[index] = color;
			}
		}

		// òg
		for (int32_t y = outline; y < height_o - outline; y++)
		{
			for (int32_t x = outline; x < width_o - outline; x++)
			{
				if (result->buffer[x + width_o * y].a == 0) continue;

				auto radius = outline * result->buffer[x + width_o * y].a / 255.0f;

				for (int32_t y_ = y - outline; y_ <= y + outline; y_++)
				{
					for (int32_t x_ = x - outline; x_ <= x + outline; x_++)
					{
						float intensity = 0.0f;

						auto distance = sqrt((x - x_) * (x - x_) + (y - y_) * (y - y_));

						if (radius >= distance)
						{
							intensity = 1.0f;
						}
						else
						{
							intensity = 1.0f - (distance - radius);
						}

						if (intensity > 1.0f) intensity = 1.0f;
						if (intensity < 0.0f) intensity = 0.0f;

						auto value = uint8_t(intensity * 255);
						if (result->buffer[x_ + width_o * y_].g < value)
						{
							result->buffer[x_ + width_o * y_].r = 255;
							result->buffer[x_ + width_o * y_].g = value;
						}
					}
				}
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
				auto baseColor = buffer[index];

				float oi = baseColor.g / 255.0f;
				float fi = baseColor.a / 255.0f;

				float a = 1.0f - (1.0f - oi) * (1.0f - fi);
				float r = (color.r / 255.0f) * fi + (outlineColor.r / 255.0f) * (1.0f - fi) * oi;
				float g = (color.g / 255.0f) * fi + (outlineColor.g / 255.0f) * (1.0f - fi) * oi;
				float b = (color.b / 255.0f) * fi + (outlineColor.b / 255.0f) * (1.0f - fi) * oi;
				
				if (a > 1.0f) a = 1.0f;
				if (a < 0.0f) a = 0.0f;

				if (a > 0.0f)
				{
					r /= a;
					g /= a;
					b /= a;
				}

				if (r > 1.0f) r = 1.0f;
				if (r < 0.0f) r = 0.0f;
				if (g > 1.0f) g = 1.0f;
				if (g < 0.0f) g = 0.0f;
				if (b > 1.0f) b = 1.0f;
				if (b < 0.0f) b = 0.0f;

				result->buffer[index].r = 255 * r;
				result->buffer[index].g = 255 * g;
				result->buffer[index].b = 255 * b;
				result->buffer[index].a = 255 * a;
			}
		}

		return result;
	}
}