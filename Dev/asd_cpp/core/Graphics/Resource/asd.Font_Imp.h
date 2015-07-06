#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

#include <Math/asd.Vector2DI.h>

#include <Graphics/Font/asd.AffLoader.h>
#include <Graphics/Font/asd.GlyphData.h>
#include "asd.Texture2D.h"
#include "asd.Font.h"
#include "../asd.DeviceObject.h"

#include <Font/asd.FontRasterizer.h>
#include <Font/asd.InstalledFontList.h>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	class Font_Imp
		: public Font
		, public DeviceObject
	{
	protected:

		Graphics*	m_graphics;

		std::shared_ptr<FontRasterizer> rasterizer;
		std::set<int32_t>				updatingTexture;
		bool		isDynamic = false;

		std::vector<std::shared_ptr<Texture2D>> m_textures;
		std::map<achar, GlyphData> m_glyphs;

		public:


		static Font_Imp* Create(Graphics* graphics, const achar* font, int32_t fontSize, Color color, int32_t outlineSize, Color outlineColor);

		Font_Imp(Graphics* graphics, std::shared_ptr<FontRasterizer> rasterizer);

		Font_Imp(Graphics* graphics, const achar* affFilePathChar, std::vector<uint8_t> data);

#if !SWIG
		/**
			@brief	内部テクスチャ用コンストラクタ
			@param	graphics	描画クラス
			@param	data		データ
			@param	size		データサイズ
			@param	textures	テクスチャ
		*/
		Font_Imp(Graphics* graphics, void* data, int32_t size, std::vector<std::shared_ptr<Texture2D>> textures);
#endif

		virtual ~Font_Imp();

		Vector2DI CalcTextureSize(const achar* text, WritingDirection writingDirection) override;

#if !SWIG
		std::shared_ptr<Texture2D> GetTexture(int index) const;

		/**
			@brief	動的に文字を追加する。
		*/
		void AddCharactorDynamically(achar c);

		/**
		@brief	動的に文字を追加する。
		*/
		void AddCharactorsDynamically(const achar* text);

		/**
			@brief	動的にテクスチャを更新する。
			@note
			文字を追加した後に実行する。
		*/
		void UpdateTextureDynamically();

		GlyphData GetGlyphData(achar c);
		bool HasGlyphData(achar c) const;

		// リロード
		void Reload(const achar* affFilePathChar, std::vector<uint8_t> data);
#endif

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}