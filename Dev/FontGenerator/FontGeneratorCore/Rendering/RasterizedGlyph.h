#pragma once
#include <vector>
#include <memory>
#include "Span.h"
#include "Color.h"

namespace FontGenerator
{
	struct Span;
	typedef std::vector<Span> Spans;

	class RasterizedGlyph
	{
	private:
		std::vector<Color> buffer;
		int width, height;

		RasterizedGlyph(int width, int height);

	public:
		typedef std::shared_ptr<RasterizedGlyph> Ptr;

		static RasterizedGlyph::Ptr FromSpans(
			Spans spans,
			int width,
			int height,
			int baselineY);

		RasterizedGlyph::Ptr PaintColor(Color color);

		std::vector<Color> GetBuffer() const
		{
			return buffer;
		}

		int GetWidth() const
		{
			return width;
		}

		int GetHeight() const
		{
			return height;
		}
	};
}