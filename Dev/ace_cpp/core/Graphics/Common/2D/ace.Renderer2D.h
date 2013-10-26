
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include <ace.common.Base.h>

#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector2DI.h>
#include <Graphics/ace.Color.h>

#include "../../../ace.Core.Base.h"

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
		virtual void AddSprite(Vector2DF positions[4], Color colors[4], Vector2DF uv[4], Texture2D* texture, eAlphaBlend alphaBlend, int32_t priority) = 0;
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

}