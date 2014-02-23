#pragma once
#include <ace.common.Base.h>
#include <vector>
#include <memory>
#include <freetype/ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include "Glyph.h"

namespace FontGenerator
{
	class Font
	{
	private:
		FT_Library m_library;
		FT_Face m_face;
		int m_fontSize;

	public:
		typedef std::shared_ptr<Font> Ptr;

		Font(ace::astring fontPath);
		~Font();
		std::vector<Glyph::Ptr> GetGlyphs(std::vector<ace::achar>& charactors);

		int GetFontSize() const;
		void SetFontSize(int value);

		int GetFontHeight() const;
	};
}