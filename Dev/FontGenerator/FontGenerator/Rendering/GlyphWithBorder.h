#pragma once
#include <cstdint>
#include "Glyph.h"

namespace FontGenerator
{
	class GlyphWithBorder
	{
	private:
		Glyph::Ptr m_body;
		Glyph::Ptr m_outline;

	public:
		GlyphWithBorder(Glyph::Ptr& body, Glyph::Ptr outline);

		Glyph::Ptr GetBody() const;

		Glyph::Ptr GetOutline() const;

		int GetAdvance() const;

		void Draw(std::int32_t* buffer, int width, int height, int x, int y);
	};
}