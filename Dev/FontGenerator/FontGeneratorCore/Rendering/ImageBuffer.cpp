#include "ImageBuffer.h"
#include "Span.h"
#include <vector>
#include <Math\ace.RectI.h>

using namespace std;

namespace FontGenerator
{
	ImageBuffer::ImageBuffer(Font& font, int sheetSize)
		: ascender(font.GetAscender())
		, descender(font.GetDescender())
		, height(font.GetFontHeight())
		, penX(0)
		, baseLineY(0)
		, sheetNum(0)
		, buffers(vector<vectorPtr>())
		, sheetSize(sheetSize)
	{
		baseLineY = font.GetAscender();
		buffers.push_back(CreateBuffer(sheetSize));
	}

	ace::GlyphData ImageBuffer::DrawGlyph(Glyph::Ptr glyph)
	{
		const int SPACE_SIZE = 1;

		auto advance = glyph->GetAdvance();
		if (penX + advance > sheetSize)
		{
			if (baseLineY - descender + height > sheetSize)
			{
				sheetNum++;
				buffers.push_back(CreateBuffer(sheetSize));
				penX = 0;
				baseLineY = ascender;
			}
			else
			{
				penX = 0;
				baseLineY += height + SPACE_SIZE;
			}
		}

		glyph->Draw(buffers[sheetNum]->data(), sheetSize, sheetSize, penX, baseLineY);

		auto src = ace::RectI(penX, baseLineY - ascender, advance, height);
		auto result = ace::GlyphData(glyph->GetCharactor(), sheetNum, src);

		penX += advance + SPACE_SIZE;

		return result;
	}

	ImageBuffer::vectorPtr ImageBuffer::CreateBuffer(int sheetSize)
	{
		return make_shared<vector<int>>(sheetSize*sheetSize, 0);
	}

	vector<ImageBuffer::vectorPtr>& ImageBuffer::GetBuffers()
	{
		return buffers;
	}

	ace::GlyphData ImageBuffer::Draw(Glyph::Ptr glyph)
	{
		auto advance = glyph->GetAdvance();
		if (penX + advance > sheetSize)
		{
			if (baseLineY - descender + height > sheetSize)
			{
				sheetNum++;
				buffers.push_back(CreateBuffer(sheetSize));
				penX = 0;
				baseLineY = ascender;
			}
			else
			{
				penX = 0;
				baseLineY += height;
			}
		}

		auto rasterized = glyph->Rasterize();
		DrawRasterizedGlyph(rasterized);

		auto src = ace::RectI(penX, baseLineY - ascender, advance, height);
		auto result = ace::GlyphData(glyph->GetCharactor(), sheetNum, src);

		penX += advance;

		return result;
	}

	void ImageBuffer::DrawRasterizedGlyph(RasterizedGlyph::Ptr glyph)
	{
		auto dst = buffers[sheetNum];
		auto src = glyph->GetBuffer();
		int width = glyph->GetWidth();
		int height = glyph->GetHeight();

		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < width; x++)
			{
				int srcIndex = y * width + x;
				int dstIndex = y * sheetSize + x;
				(*dst)[dstIndex] = src[srcIndex].GetInt();
			}
		}
	}
}