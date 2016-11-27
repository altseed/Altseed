
#pragma once

#include <vector>
#include <memory>
#include "../../asd.CoreToEngine.h"

namespace asd
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
		friend class PostEffectGrayScale;
		friend class PostEffectGaussianBlur;
		friend class PostEffectLightBloom;
		friend class PostEffectSepia;

	private:
		std::shared_ptr<CorePostEffect>	m_corePostEffect;

		CorePostEffect* GetCoreObject() const;

		bool	isEnabled = true;

	protected:
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

	public:
		PostEffect();
		virtual ~PostEffect();

		/**
			@brief	このポストエフェクトが有効かどうか、取得する。
			@return	有効かどうか
		*/
		bool GetIsEnabled() const;

		/**
		@brief	このポストエフェクトが有効かどうか、設定する。
		@param	value	有効かどうか
		*/
		void SetIsEnabled(bool value);
	};
}
