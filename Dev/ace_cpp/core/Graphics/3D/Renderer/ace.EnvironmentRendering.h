
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

		float G1V(float dotNV, float k);
		void CalcGGX_WithoutF0(const Vector3DF& normal, const Vector3DF& light, const Vector3DF& view, float roughness, float& v1, float& v2);
		void IntegrateGGX_WithoutF0(float NoV, float roughness, float& v1, float& v2);

		void CalcIntegratedGGX_WithoutF0(int32_t width, int32_t height, std::vector<Color>& dst);

	public:
		EnvironmentRendering(Graphics* graphics, std::shared_ptr<ace::VertexBuffer_Imp> vb, std::shared_ptr<ace::IndexBuffer_Imp> ib);
		virtual ~EnvironmentRendering();

		void Render(RenderedCameraObject3DProxy* cameraP, RenderingCommandHelper* helper, RenderTexture2D_Imp* gb0, RenderTexture2D_Imp* gb1, RenderTexture2D_Imp* gb2, RenderTexture2D_Imp* gb3,
			CubemapTexture* diffuseColor, CubemapTexture* specularColor);
	};
}