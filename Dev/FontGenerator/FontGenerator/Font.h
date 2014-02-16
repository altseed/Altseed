#pragma once

#include <ace.common.Base.h>
#include <vector>
#include <freetype/ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace FontGenerator
{
	class Font
	{
	private:
		FT_Library m_library;
		FT_Face m_face;
		int m_fontSize;
		std::vector<FT_Glyph> m_cashForRelease;

	public:
		Font(ace::astring fontPath);
		~Font();
		std::vector<FT_BitmapGlyph> GetGlyphs(std::vector<ace::achar>& charactors);

		int GetFontSize() const;
		void SetFontSize(int value);
	};
}