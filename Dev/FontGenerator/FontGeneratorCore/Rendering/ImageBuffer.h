#pragma once
#include <vector>
#include <memory>
#include "Font.h"
#include "RasterizedGlyph.h"
#include <Graphics/Font/asd.GlyphData.h>

namespace FontGenerator
{
	class ImageBuffer
	{
		typedef std::shared_ptr<std::vector<int>> vectorPtr;

	public:
		ImageBuffer(Font& font, int sheetSize);
		std::vector<vectorPtr>& GetBuffers();

		asd::GlyphData Draw(Glyph::Ptr glyph, Color color, BorderSetting::Ptr border);

	private:

		int ascender, descender, height;
		int sheetSize;
		int sheetNum, penX, baseLineY;
		std::vector<vectorPtr> buffers;

		static vectorPtr CreateBuffer(int sheetSize);
		void DrawRasterizedGlyph(RasterizedGlyph::Ptr glyph, int penX, int penY);
	};
}