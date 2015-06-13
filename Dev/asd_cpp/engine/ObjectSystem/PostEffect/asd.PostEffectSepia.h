#pragma once

#include<memory>
#include <vector>
#include"asd.PostEffect.h"


namespace asd
{

	/**
	@brief	セピア調化するポストエフェクトクラス
	*/

	class PostEffectSepia : public PostEffect
	{
	private:
		std::shared_ptr<asd::Material2D>	material2d;
	public:
		PostEffectSepia();
		virtual ~PostEffectSepia() = default;

		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;
	};

}