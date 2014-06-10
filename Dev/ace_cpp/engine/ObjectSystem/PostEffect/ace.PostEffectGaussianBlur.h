#pragma once

#include<memory>
#include <vector>
#include"ace.PostEffect.h"


namespace ace
{

	/**
	@brief	ガウスぼかしをかけるポストエフェクトクラス
	*/




	class PostEffectGaussianBlur : public PostEffect
	{
	private:

		std::shared_ptr<ace::Shader2D>		m_shaderX, m_shaderY;
		std::shared_ptr<ace::Material2D>	m_material2dX, m_material2dY;
		ace::Graphics *m_graphics;

		std::shared_ptr<RenderTexture2D>	m_tempTexture;

		PostEffectGaussianBlur() = default;
		
		float intensity = 5.0f;

	public:
		PostEffectGaussianBlur(Graphics *g);

		virtual ~PostEffectGaussianBlur() = default;

		/**
		@brief ぼかしの強さを設定する。初期値は0.5
		@detail 実質的にはガウス関数の分散にあたる値の平方根。1.0fから5.0fあたりが適当
		*/
		void SetIntensity(float const value){ if (value <= 0.0f){ return; }intensity = value; }

		
		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;
	};

}