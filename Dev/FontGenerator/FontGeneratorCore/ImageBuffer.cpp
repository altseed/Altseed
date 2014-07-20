#include "ImageBuffer.h"
#include "Rendering\Span.h"
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

		glyph->Draw(buffers[sheetNum]->data(), sheetSize, sheetSize, penX, baseLineY);

		auto src = ace::RectI(penX, baseLineY - ascender, advance, height);
		auto result = ace::GlyphData(glyph->GetCharactor(), sheetNum, src);

		penX += advance;

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
}