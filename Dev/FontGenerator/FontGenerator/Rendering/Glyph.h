#pragma once
#include <vector>
#include <memory>
#include "Color.h"
#include <freetype/ft2build.h>
#include FT_GLYPH_H

namespace FontGenerator
{
	struct Span
	{
		Span() {}
		Span(int _x, int _y, int _width, int _coverage)
			: x(_x), y(_y), width(_width), coverage(_coverage)
		{
		}

		int x, y, width, coverage;
	};

	typedef std::vector<Span> Spans;

	class Glyph
	{
	private:
		FT_Library* m_library;
		FT_OutlineGlyph m_glyph;
		Color m_color;

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

		/**
			@brief	このグリフの描画幅を取得する。
		*/
		int GetAdvance() const;

		/**
			@brief	このグリフを描画する際の色を設定する。
		*/
		void SetColor(Color color);

		/**
			@brief	このグリフを輪郭線用に処理したグリフを新しく生成して返す。
		*/
		Glyph::Ptr Border(float width, Color color);

		/**
			@brief	このグリフを太字にしたグリフを新しく生成して返す。
		*/
		Glyph::Ptr Enbolden(float weight);

		/**
			@brief	指定したバッファにこのグリフを描画する。
			@param	buffer	描画先となるバッファ
					width	バッファの幅
					height	バッファの高さ
					x		描画原点の x座標
					y		ベースラインの y座標
		*/
		void Draw(int32_t* buffer, int width, int height, int x, int y);
	};
}