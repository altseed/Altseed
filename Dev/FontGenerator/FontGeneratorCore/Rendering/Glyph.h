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
	class RasterizedGlyph;

	class Glyph
	{
	private:
		FT_Library* m_library;
		FT_Face m_face;
		FT_OutlineGlyph m_glyph;
		Color m_color;
		BorderSetting::Ptr m_border;
		wchar_t m_charactor;

		/**
			@brief	保持するfreetypeのグリフを直接指定して初期化する。
			@param	library	freetypeのライブラリ オブジェクト
					glyph	グリフ
		*/
		Glyph(FT_Library& library, FT_OutlineGlyph& glyph);

	public:
		typedef std::shared_ptr<Glyph> Ptr;

		Glyph(FT_Library& library, FT_Face& face, wchar_t ch);
		~Glyph();

		wchar_t GetCharactor() const;

		/**
			@brief	このグリフの描画幅を取得する。
		*/
		int GetAdvance() const;

		/**
			@brief	このグリフを描画する際の色を設定する。
		*/
		void SetColor(Color color);

		/**
			@brief	指定したバッファにこのグリフを描画する。
			@param	buffer	描画先となるバッファ
					width	バッファの幅
					height	バッファの高さ
					x		描画原点の x座標
					y		ベースラインの y座標
		*/
		void Draw(int32_t* buffer, int width, int height, int x, int y);

		BorderSetting::Ptr GetBorderSetting() const;
		void SetBorderSetting(BorderSetting::Ptr value);

		RasterizedGlyph::Ptr Rasterize();
	};
}