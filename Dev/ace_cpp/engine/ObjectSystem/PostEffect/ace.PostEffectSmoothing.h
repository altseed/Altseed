#pragma once

#include<memory>
#include <vector>
#include"ace.PostEffect.h"


namespace ace
{

	/**
	@brief	平滑化ぼかしをかけるポストエフェクトクラス
	*/




	class PostEffectSmoothing : public PostEffect
	{
	private:

		std::shared_ptr<ace::Shader2D>		m_shaderX, m_shaderY;
		std::shared_ptr<ace::Material2D>	m_material2dX, m_material2dY;
		ace::Graphics *m_graphics;
		PostEffectSmoothing() = default;



	public:
		PostEffectSmoothing(Graphics *g);

		virtual ~PostEffectSmoothing() = default;

		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;
	};

}