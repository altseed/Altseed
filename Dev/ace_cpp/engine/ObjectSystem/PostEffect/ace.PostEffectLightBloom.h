#pragma once

#include<memory>
#include <vector>
#include"ace.PostEffect.h"

#define DOWNSAMPLE 1
namespace ace
{

	/**
		@brief	輝度の高い画素のみをぼかして加算するポストエフェクト
	*/
	class PostEffectLightBloom : public PostEffect
	{
	private:
		std::shared_ptr<ace::Material2D>	material_copy, material2dX, material2dY, material2dY_Sum, materialSum, downsample;

#if defined(DOWNSAMPLE)
		std::shared_ptr<RenderTexture2D>	tempTexture0;
		std::shared_ptr<RenderTexture2D>	tempTexture1;
		std::shared_ptr<RenderTexture2D>	tempTexture2;
		std::shared_ptr<RenderTexture2D>	tempTexture3;

		std::shared_ptr<RenderTexture2D>	downsampledTexture0;
		std::shared_ptr<RenderTexture2D>	downsampledTexture1;
		std::shared_ptr<RenderTexture2D>	downsampledTexture2;
		std::shared_ptr<RenderTexture2D>	downsampledTexture3;

#else
		std::shared_ptr<RenderTexture2D>	copiedTexture;
		std::shared_ptr<RenderTexture2D>	tempTexture;
#endif
		float intensity = 5.0f;
		float threshold = 1.0f;
		float exposure = 1.0f;

	public:
		PostEffectLightBloom();
		virtual ~PostEffectLightBloom() = default;

		/**
		@brief ぼかしの強さを設定する。
		@note
		実質的にはガウス関数の分散にあたる値の平方根。
		*/
		void SetIntensity(float const value){ if (value <= 0.0f){ return; }intensity = value; }

		/**
		@brief	ぼかすピクセルの閾値を設定する。
		@note
		この閾値を超えた値がぼかされ加算される。
		*/
		void SetThreshold(float const value){ threshold = value; }

		/**
			@brief	露光の強さを指定する。
			@note
			この値が高いほどぼかされる値の輝度が高くなる。
		*/
		void SetExposure(float const value) { exposure = value; }

		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;
	};

}