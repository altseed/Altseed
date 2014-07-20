#include <vector>
#include <memory>
#include "Rendering\Font.h"
#include <Graphics\Font\ace.GlyphData.h>

namespace FontGenerator
{
	class ImageBuffer
	{
		typedef std::shared_ptr<std::vector<int>> vectorPtr;

	public:
		ImageBuffer(Font& font, int sheetSize);
		ace::GlyphData DrawGlyph(Glyph::Ptr glyph);
		std::vector<vectorPtr>& GetBuffers();

	private:

		int ascender, descender, height;
		int sheetSize;
		int sheetNum, penX, baseLineY;
		std::vector<vectorPtr> buffers;

		static vectorPtr CreateBuffer(int sheetSize);
		void DrawRasterizedGlyph(Glyph::Ptr glyph);
	};
}