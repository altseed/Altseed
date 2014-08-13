
#pragma once

#include <ace.common.Base.h>

#include "../../../ace.Core.Base.h"
#include "../../../ace.Core.Base_Imp.h"

namespace ace
{
	class EnvironmentRendering
	{
	private:
		std::shared_ptr<ace::NativeShader_Imp>	environmentShader;

		std::shared_ptr<ace::VertexBuffer_Imp>	vertexBuffer;
		std::shared_ptr<ace::IndexBuffer_Imp>	indexBuffer;

	public:
		EnvironmentRendering(Graphics* graphics, std::shared_ptr<ace::VertexBuffer_Imp> vb, std::shared_ptr<ace::IndexBuffer_Imp> ib);
		virtual ~EnvironmentRendering();

		void Render(RenderedCameraObject3DProxy* cameraP, RenderingCommandHelper* helper, RenderTexture2D_Imp* gb0, RenderTexture2D_Imp* gb1, RenderTexture2D_Imp* gb2, RenderTexture2D_Imp* gb3,
			CubemapTexture* diffuseColor, CubemapTexture* specularColor);
	};
}