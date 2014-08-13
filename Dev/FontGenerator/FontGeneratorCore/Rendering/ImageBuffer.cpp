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

	ImageBuffer::vectorPtr ImageBuffer::CreateBuffer(int sheetSize)
	{
		return make_shared<vector<int>>(sheetSize*sheetSize, 0);
	}

	vector<ImageBuffer::vectorPtr>& ImageBuffer::GetBuffers()
	{
		return buffers;
	}

	ace::GlyphData ImageBuffer::Draw(Glyph::Ptr glyph, Color color, BorderSetting::Ptr border)
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

		auto rasterized = glyph->Rasterize();
		rasterized = rasterized->PaintColor(color);
		DrawRasterizedGlyph(rasterized, penX, baseLineY - ascender);

		auto src = ace::RectI(penX, baseLineY - ascender, advance, height);
		auto result = ace::GlyphData(glyph->GetCharactor(), sheetNum, src);

		penX += advance + SPACE_SIZE;

		return result;
	}

	void ImageBuffer::DrawRasterizedGlyph(RasterizedGlyph::Ptr glyph, int penX, int penY)
	{
		auto dst = buffers[sheetNum];
		auto src = glyph->GetBuffer();
		int width = glyph->GetWidth();
		int height = glyph->GetHeight();

		for (size_t y = 0; y < height && y + penY < sheetSize; y++)
		{
			for (size_t x = 0; x < width && x + penX < sheetSize; x++)
			{
				int srcIndex = y * width + x;
				int dstIndex = (penY + y) * sheetSize + penX + x;
				(*dst)[dstIndex] = src[srcIndex].GetInt();
			}
		}
	}
}