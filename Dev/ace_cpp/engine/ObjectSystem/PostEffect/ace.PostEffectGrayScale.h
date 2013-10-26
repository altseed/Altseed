#pragma once

#include<memory>
#include <vector>
#include"ace.PostEffect.h"

// #include <ace.h>

namespace ace
{

	/**
	@brief	グレースケール化するポストエフェクトクラス
	*/

	class PostEffectGrayScale : public PostEffect
	{
	private:

		std::shared_ptr<ace::Shader2D>		m_shader;
		std::shared_ptr<ace::Material2D>	m_material2d;
		PostEffectGrayScale() = default;
	public:
		PostEffectGrayScale(Graphics *g);

		virtual ~PostEffectGrayScale() = default;

		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;
	};

}