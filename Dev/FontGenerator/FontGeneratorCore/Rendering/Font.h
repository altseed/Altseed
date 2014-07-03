#pragma once
#include <ace.common.Base.h>
#include <vector>
#include <memory>
#include <ft2build.h>
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

		/**
			@brief	フォントのascenderの値(ベースラインより上の高さ)を取得します。
		*/
		int GetAscender() const;

		/**
			@brief	フォントのdescenderの値(ベースラインより下の高さ)を取得します。戻り値は負の値です。
		*/
		int GetDescender() const;
	};
}