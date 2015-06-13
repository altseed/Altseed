#pragma once

#include<memory>
#include <vector>
#include"asd.PostEffect.h"


namespace asd
{

	/**
	@brief	ガウスぼかしをかけるポストエフェクトクラス
	*/
	class PostEffectGaussianBlur : public PostEffect
	{
	private:
		std::shared_ptr<Material2D>	material2dX;
		std::shared_ptr<Material2D>	material2dY;
		std::shared_ptr<RenderTexture2D>	tempTexture;

		float intensity = 5.0f;

	public:
		PostEffectGaussianBlur();
		virtual ~PostEffectGaussianBlur() = default;

		/**
		@brief ぼかしの強さを設定する。
		@detail 実質的にはガウス関数の分散にあたる値の平方根
		*/
		void SetIntensity(float const value){ if (value <= 0.0f){ return; }intensity = value; }

		
		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;
	};

}