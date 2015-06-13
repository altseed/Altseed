#pragma once

#include<memory>
#include <vector>
#include"asd.PostEffect.h"

// #include <asd.h>

namespace asd
{

	/**
	@brief	グレースケール化するポストエフェクトクラス
	*/

	class PostEffectGrayScale : public PostEffect
	{
	private:
		std::shared_ptr<asd::Material2D>	material2d;

	public:
		PostEffectGrayScale();
		virtual ~PostEffectGrayScale() = default;

		virtual void OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src) override;
	};

}