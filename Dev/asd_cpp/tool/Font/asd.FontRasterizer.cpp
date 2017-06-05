
#include "asd.FontRasterizer.h"

#if !(defined(_CONSOLE_GAME))

namespace asd
{
	FontRasterizer::Span::Span()
		: x(0), y(0), width(0), coverage(0)
	{
	}

	FontRasterizer::Span::Span(int x, int y, int width, int coverage)
		: x(x), y(y), width(width), coverage(coverage)
	{
	}

	void FontRasterizer::RasterCallback(
		const int y,
		const int count,
		const FT_Span * const spans,
		void * const user)
	{
		Spans *sptr = (Spans *) user;
		for (int i = 0; i < count; ++i)
		{
			sptr->push_back(Span(spans[i].x, y, spans[i].len, spans[i].coverage));
		}
	}

	void FontRasterizer::RenderSpans(
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

	FontRasterizer::Font::Font(FT_Library library, FT_Face face, int32_t fontSize, std::shared_ptr<std::vector<uint8_t>> buf)
		: library(library)
		, face(face)
		, fontSize(fontSize)
		, buf(buf)
	{
		FT_Set_Pixel_Sizes(face, fontSize, fontSize);
	}

	FontRasterizer::Font::~Font()
	{
		FT_Done_Face(face);
		FT_Done_FreeType(library);
	}

	std::shared_ptr<FontRasterizer::Font> FontRasterizer::Font::Create(void* data, int32_t size, int32_t fontSize)
	{
		FT_Library library = nullptr;
		auto error = FT_Init_FreeType(&library);
		if (error != 0) return nullptr;

		auto buf = std::make_shared<std::vector<uint8_t>>();
		buf->resize(size);
		memcpy(buf->data(), data, size);

		FT_Face face = nullptr;
		error = FT_New_Memory_Face(library, (uint8_t*)buf->data(), size, 0, &face);
		if (error != 0)
		{
			FT_Done_FreeType(library);
			return nullptr;
		}

		return std::make_shared<FontRasterizer::Font>(library, face, fontSize, buf);
	}

	FontRasterizer::Glyph::Glyph(std::shared_ptr<Font> font, achar charactor, FT_OutlineGlyph glyph)
		: font(font)
		, charactor(charactor)
		, glyph(glyph)
	{
	}

	FontRasterizer::Glyph::~Glyph()
	{
		FT_Done_Glyph(&glyph->root);
	}

	void FontRasterizer::Glyph::Rasterize(std::vector<Color>& data, int32_t& width, int32_t& height, int32_t outlineSize, Color color, Color outlineColor)
	{
		Spans spans;
		FT_Library lib = font->GetLibrary();
		RenderSpans(lib, &glyph->outline, &spans);

		int width_ = GetAdvance();
		int height_ = font->GetFontHeight();
		int baselineY_ = font->GetAscender();

		auto width_o = (width_ + outlineSize * 2);
		auto height_o = (height_ + outlineSize * 2);

		data.resize(width_o * height_o, Color(0, 0, 0, 0));

		// コピー
		for (auto& s : spans)
		{
			int yIndex = baselineY_ - s.y;
			if (yIndex < 0 || yIndex >= height_) continue;

			for (int w = 0; w < s.width; w++)
			{
				int xIndex = s.x + w;
				if (xIndex < 0) continue;
				if (xIndex >= width_) break;

				Color color(0, 0, 255, s.coverage);

				int index = ((yIndex + outlineSize)) * width_o + (xIndex + outlineSize);

				data[index] = color;
			}
		}

		// 枠
		if (outlineSize > 0)
		{
			for (int32_t y = outlineSize; y < height_o - outlineSize; y++)
			{
				for (int32_t x = outlineSize; x < width_o - outlineSize; x++)
				{
					if (data[x + width_o * y].A == 0) continue;

					auto radius = outlineSize * data[x + width_o * y].A / 255.0f;

					for (int32_t y_ = y - outlineSize; y_ <= y + outlineSize; y_++)
					{
						for (int32_t x_ = x - outlineSize; x_ <= x + outlineSize; x_++)
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
							if (data[x_ + width_o * y_].G < value)
							{
								data[x_ + width_o * y_].R = 255;
								data[x_ + width_o * y_].G = value;
							}
						}
					}
				}
			}
		}

		// 色
		for (size_t i = 0; i < data.size(); i++)
		{
			auto baseColor = data[i];

			float oi = baseColor.G / 255.0f;
			float fi = baseColor.A / 255.0f;

			float a = 1.0f - (1.0f - oi) * (1.0f - fi);
			float r = (color.R / 255.0f) * fi + (outlineColor.R / 255.0f) * (1.0f - fi) * oi;
			float g = (color.G / 255.0f) * fi + (outlineColor.G / 255.0f) * (1.0f - fi) * oi;
			float b = (color.B / 255.0f) * fi + (outlineColor.B / 255.0f) * (1.0f - fi) * oi;

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

			data[i].R = 255 * r;
			data[i].G = 255 * g;
			data[i].B = 255 * b;
			data[i].A = 255 * a;
		}
		
		width = width_o;
		height = height_o;
	}

	std::shared_ptr<FontRasterizer::Glyph> FontRasterizer::Glyph::CreateGlyph(std::shared_ptr<FontRasterizer::Font> font, achar charactor)
	{
		auto index = FT_Get_Char_Index(font->GetFace(), charactor);
		FT_Load_Glyph(font->GetFace(), index, FT_LOAD_NO_BITMAP);

		FT_Glyph g;
		FT_Get_Glyph(font->GetFace()->glyph, &g);

		if (g->format != FT_GLYPH_FORMAT_OUTLINE) return nullptr;

		auto og = reinterpret_cast<FT_OutlineGlyph>(g);

		return std::make_shared<Glyph>(font, charactor, og);
	}

	FontRasterizer::Image::Image(int32_t size)
	{
		Buffer.resize(size * size, Color(0, 0, 0, 0));
	}

	FontRasterizer::FontRasterizer()
	{
	}

	FontRasterizer::~FontRasterizer()
	{

	}

	bool FontRasterizer::Initialize(void* fontfileData, int32_t fontfileSize, int32_t fontSize, int32_t outlineSize, Color color, Color outlineColor, int32_t imageSize)
	{
		font.reset();
		glyphs.clear();
		images.clear();
		glyphImages.clear();

		font = Font::Create(fontfileData, fontfileSize, fontSize);
		if (font == nullptr) return false;

		this->outlineSize = outlineSize;
		this->color = color;
		this->outlineColor = outlineColor;
	
		this->imageSize = imageSize;

		images.push_back(std::make_shared<Image>(imageSize));

		currentX = Space;
		currentY = Space;
		currentIndex = 0;
		currentHeight = 0;

		return true;
	}

	FontRasterizer::GlyphImage FontRasterizer::AddGlyph(achar charactor)
	{
		if (glyphs.count(charactor) > 0) return glyphImages[charactor];

		auto glyph = Glyph::CreateGlyph(font, charactor);
		glyphs[charactor] = glyph;

		auto advance = glyph->GetAdvance();
		auto descender = font->GetDescender();
		auto baseLineY = font->GetAscender();

		std::vector<Color> data;
		int32_t width;
		int32_t height;

		glyph->Rasterize(data, width, height, outlineSize, color, outlineColor);

		int32_t offsetX = 0;
		int32_t offsetY = 0;

		if (currentX + width + Space <= imageSize &&
			currentY + height + Space <= imageSize)
		{
			offsetX = currentX;
			offsetY = currentY;

			currentX += width + Space;

			currentHeight = Max(height, currentHeight);
		}
		else if (currentY + currentHeight + Space + height + Space <= imageSize)
		{
			currentX = Space;
			currentY += currentHeight + Space;

			offsetX = currentX;
			offsetY = currentY;

			currentX += width + Space;
			currentHeight = height;
		}
		else
		{
			currentX = Space;
			currentY = Space;
			currentIndex++;

			offsetX = currentX;
			offsetY = currentY;

			currentX += width + Space;
			currentHeight = height;

			images.push_back(std::make_shared<Image>(imageSize));
		}

		GlyphImage gi;
		gi.Src.X = offsetX;
		gi.Src.Y = offsetY;
		gi.Src.Width = width;
		gi.Src.Height = height;
		gi.Index = currentIndex;

		auto& img = images[gi.Index];

		for (size_t y = gi.Src.Y; y < gi.Src.Y + gi.Src.Height; y++)
		{
			for (size_t x = gi.Src.X; x < gi.Src.X + gi.Src.Width; x++)
			{
				img->Buffer[x + imageSize * y] = data[(x - gi.Src.X) + (y - gi.Src.Y) * width];
			}
		}

		glyphImages[charactor] = gi;

		return gi;
	}
}

#endif