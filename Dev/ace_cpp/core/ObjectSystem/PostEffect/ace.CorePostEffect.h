
#pragma once

#include "../../ace.ReferenceObject.h"

namespace ace
{
	/**
		@brief	ポストエフェクトの内部クラス
	*/
	class CorePostEffect
		: public IReference
	{
	protected:
		CorePostEffect(){}
		virtual ~CorePostEffect(){}
	public:

		/**
		@brief	マテリアルを用いてテクスチャに画像を描画する。
		@param	target		描画先
		@param	material	マテリアル
		*/
		virtual void DrawOnTexture2DWithMaterial(RenderTexture2D* target, Material2D* material) = 0;

		virtual const char* GetLightBloomShader(GraphicsType graphicsType) const = 0;

	};
}