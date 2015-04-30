
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>

#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector2DI.h>
#include <Math/ace.Matrix33.h>

#include <Graphics/ace.Color.h>

#include "../../ace.Core.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer {
	class Manager;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

	/**
	@brief	2D描画全てを管理するクラスのインターフェース
	*/
	class Renderer2D
	{
	protected:
		Renderer2D();

	public:
		virtual ~Renderer2D();
		
		/**
			@brief	描画領域を設定する。
			@param	area	描画領域
			@param	angle	描画領域を中心とした回転
		*/
		virtual void SetArea(const RectF& area, float angle) = 0;

		/**
			@brief	キャッシュを描画する。
		*/
		virtual void DrawCache() = 0;

		/**
			@brief	キャッシュを消去する。
		*/
		virtual void ClearCache() = 0;

		/**
			@brief	スプライト描画をキャッシュに追加する。
			@param	positions	頂点座標
			@param	colors		頂点色
			@param	uv			頂点UV
			@param	texture		描画に使用するテクスチャ(nullで無地)
			@param	priority	描画優先度
		*/
		virtual void AddSprite(Vector2DF positions[4], Color colors[4], Vector2DF uv[4], Texture2D* texture, AlphaBlendMode alphaBlend, int32_t priority, TextureFilterType filter = TextureFilterType::Nearest, TextureWrapType wrap = TextureWrapType::Clamp) = 0;

		virtual void AddText(Matrix33& parentMatrix, Matrix33& matrix, Vector2DF centerPosition, bool turnLR, bool turnUL, Color color, Font* font, const achar* text, WritingDirection writingDirection, AlphaBlendMode alphaBlend, int32_t priority, TextureFilterType filter = TextureFilterType::Nearest, TextureWrapType wrap = TextureWrapType::Clamp) = 0;
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}