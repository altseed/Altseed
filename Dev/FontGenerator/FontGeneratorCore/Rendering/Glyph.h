#pragma once
#include <vector>
#include <memory>
#include "Color.h"
#include "Setting.h"
#include "RasterizedGlyph.h"
#include <ft2build.h>
#include FT_GLYPH_H

namespace FontGenerator
{
	class Font;

	class Glyph
	{
	private:
		Font& font;
		FT_OutlineGlyph m_glyph;
		wchar_t m_charactor;

	public:
		typedef std::shared_ptr<Glyph> Ptr;

		Glyph(Font& font, wchar_t charctor, FT_OutlineGlyph glyph);
		~Glyph();

		wchar_t GetCharactor() const;

		/**
			@brief	このグリフの描画幅を取得する。
		*/
		int GetAdvance() const;

		/**
			@brief	指定したバッファにこのグリフを描画する。
			@param	buffer	描画先となるバッファ
					width	バッファの幅
					height	バッファの高さ
					x		描画原点の x座標
					y		ベースラインの y座標
		*/
		void Draw(int32_t* buffer, int width, int height, int x, int y);

		std::shared_ptr<RasterizedGlyph> Rasterize();

		std::shared_ptr<RasterizedGlyph> RasterizeWithOutline(int32_t outline);
	};
}