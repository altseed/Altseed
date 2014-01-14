
#pragma once

#include <vector>
#include <memory>
#include "../../ace.CoreToEngine.h"

namespace ace
{
	class Engine;

	/**
		@brief	ポストエフェクトを適用するクラス
	*/
	class PostEffect
	{
		friend class Engine;
		friend class Layer;
		friend class CameraObject3D;

	private:
		std::shared_ptr<CorePostEffect>	m_corePostEffect;

		CorePostEffect* GetCoreObject() const;

	public:
		PostEffect();
		virtual ~PostEffect();

		/**
		@brief	オーバーライドして、毎フレーム描画される処理を記述できる。
		*/
		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) {}

		/**
		@brief	マテリアルを用いてテクスチャに画像を描画する。
		@param	target		描画先
		@param	material	マテリアル
		*/
		void DrawOnTexture2DWithMaterial(std::shared_ptr<RenderTexture2D> target, std::shared_ptr<Material2D> material);
	};
}
