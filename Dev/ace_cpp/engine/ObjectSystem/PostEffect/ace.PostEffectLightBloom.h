#pragma once

#include<memory>
#include <vector>
#include"ace.PostEffect.h"


namespace ace
{

	/**
		@brief	輝度の高い画素のみをぼかして加算するポストエフェクト
	*/
	class PostEffectLightBloom : public PostEffect
	{
	private:

		std::shared_ptr<ace::Shader2D>		m_shader, m_shaderX, m_shaderY;
		std::shared_ptr<ace::Material2D>	m_material, m_material2dX, m_material2dY;
		ace::Graphics *m_graphics;

		std::shared_ptr<RenderTexture2D>	m_tempTexture;
		std::shared_ptr<RenderTexture2D>	m_copiedTexture;

		PostEffectLightBloom() = default;

		float intensity = 5.0f;
		float threshold = 1.0f;
		float power = 1.0f;
	public:
		PostEffectLightBloom(Graphics *g);

		virtual ~PostEffectLightBloom() = default;

		/**
		@brief ぼかしの強さを設定する。
		@note
		実質的にはガウス関数の分散にあたる値の平方根。1.0fから5.0fあたりが適当
		*/
		void SetIntensity(float const value){ if (value <= 0.0f){ return; }intensity = value; }

		/**
		@brief	ぼかすピクセルの閾値を設定する。
		@note
		この閾値を超えた値がぼかされ加算される。
		*/
		void SetThreshold(float const value){ threshold = value; }

		/**
			@brief	ぼかされた値を加算する強さを指定する。
			@note
			この値とぼかされた値を乗算した値が加算される。
		*/
		void SetPower(float const value) { power = value; }

		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;
	};

}