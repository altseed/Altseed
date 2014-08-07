
#pragma once

#include <ace.common.Base.h>

#include "../../../ace.Core.Base.h"
#include "../../../ace.Core.Base_Imp.h"

namespace ace
{
	class SSAO
	{
	private:
		std::shared_ptr<ace::NativeShader_Imp>	ssaoShader;
		std::shared_ptr<ace::NativeShader_Imp>	ssaoBlurXShader;
		std::shared_ptr<ace::NativeShader_Imp>	ssaoBlurYShader;

	public:
		SSAO(Graphics* graphics);
		virtual ~SSAO();

		bool IsEnabled();

		void Draw(RenderingCommandHelper* helper, RenderedCameraObject3DProxy* cameraP, std::shared_ptr<ace::VertexBuffer_Imp> vb, std::shared_ptr<ace::IndexBuffer_Imp> ib);
		float Radius = 0.1f;
		float Bias = 0.001f;
		float Intensity = 1.0f;
		float FarPlain = 800.0f;
	};
}
