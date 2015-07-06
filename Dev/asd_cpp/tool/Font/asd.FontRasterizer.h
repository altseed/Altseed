#pragma once

#include <asd.common.Base.h>
#include <Graphics/asd.Color.h>
#include <Graphics/asd.Graphics.Common.h>

#include <Math/asd.RectI.h>

#include <ft2build.h>
#include FT_GLYPH_H
#include FT_OUTLINE_H

namespace asd
{
	class FontRasterizer
	{
	private:
		class Font;
		class Glyph;

		struct Span
		{
		public:
			Span();
			Span(int _x, int _y, int _width, int _coverage);

			int32_t x, y, width, coverage;
		};

		typedef std::vector<Span> Spans;

		static void RasterCallback(
			const int y,
			const int count,
			const FT_Span * const spans,
			void * const user);

		static void RenderSpans(FT_Library &library, FT_Outline * const outline, Spans *spans);

		class Font
		{
		private:
			FT_Library	library;
			FT_Face		face;
			int32_t		fontSize;

		public:
			Font(FT_Library library, FT_Face face, int32_t fontSize);
			virtual ~Font();

			FT_Face GetFace() { return face; }

			FT_Library GetLibrary() { return library; }

			int32_t GetFontHeight() const
			{
				return face->size->metrics.height >> 6;
			}

			/**
			@brief	フォントのascenderの値(ベースラインより上の高さ)を取得する。
			*/
			int32_t GetAscender() const
			{
				return face->size->metrics.ascender >> 6;
			}

			/**
			@brief	フォントのdescenderの値(ベースラインより下の高さ)を取得します。戻り値は負の値です。
			*/
			int32_t GetDescender() const
			{
				return face->size->metrics.descender >> 6;
			}

			static std::shared_ptr<Font> Create(void* data, int32_t size, int32_t fontSize);
		};

		class Glyph
		{
		private:
			std::shared_ptr<Font> font;
			FT_OutlineGlyph glyph;
			achar charactor;

		public:
			Glyph(std::shared_ptr<Font> font, achar charactor, FT_OutlineGlyph glyph);
			virtual ~Glyph();

			/**
			@brief	このグリフの描画幅を取得する。
			*/
			int32_t GetAdvance() const
			{
				return glyph->root.advance.x >> 16;
			}

			void Rasterize(std::vector<Color>& data, int32_t& width, int32_t& height, int32_t outlineSize, Color color, Color outlineColor);

			static std::shared_ptr<Glyph> CreateGlyph(std::shared_ptr<Font> font, achar charactor);
		};

	public:
		class Image
		{
		public:
			std::vector<Color> Buffer;

			Image(int32_t size);
		};


		class GlyphImage
		{
		public:
			RectI	Src;
			int32_t	Index = 0;
		};

	private:
		int32_t	imageSize = 0;

		std::shared_ptr<Font>					font;
		std::map<achar, std::shared_ptr<Glyph>>	glyphs;
		std::vector<std::shared_ptr<Image>>		images;
		std::map<achar, GlyphImage>				glyphImages;

		int32_t outlineSize;
		Color color;
		Color outlineColor;

		int32_t	currentX = 0;
		int32_t	currentY = 0;
		int32_t currentIndex = 0;
		int32_t currentHeight = 0;

		const int32_t Space = 1;

	public:
		FontRasterizer();
		virtual ~FontRasterizer();

		bool Initialize(void* fontfileData, int32_t fontfileSize, int32_t fontSize, int32_t outlineSize, Color color, Color outlineColor, int32_t imageSize);

		int32_t GetImageSize() { return imageSize; }

		std::vector<std::shared_ptr<Image>>& GetImages() { return images; }

		GlyphImage AddGlyph(achar charactor);

		std::map<achar, GlyphImage>& GetGlyphImages() { return glyphImages; };
	};
}