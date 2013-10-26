#pragma once

#include<memory>
#include <vector>
#include"ace.PostEffect.h"


namespace ace
{

	/**
	@brief	セピア調化するポストエフェクトクラス
	*/

	class PostEffectSepia : public PostEffect
	{
	private:

		std::shared_ptr<ace::Shader2D>		m_shader;
		std::shared_ptr<ace::Material2D>	m_material2d;
		PostEffectSepia() = default;
	public:
		PostEffectSepia(Graphics *g);

		virtual ~PostEffectSepia() = default;

		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;
	};

}