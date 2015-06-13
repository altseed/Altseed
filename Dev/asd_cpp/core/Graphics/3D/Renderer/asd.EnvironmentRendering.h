
#pragma once

#include <asd.common.Base.h>

#include "../../../asd.Core.Base.h"
#include "../../../asd.Core.Base_Imp.h"

namespace asd
{
	class EnvironmentRendering
	{
	private:
		std::shared_ptr<asd::NativeShader_Imp>	environmentShader;

		std::shared_ptr<asd::VertexBuffer_Imp>	vertexBuffer;
		std::shared_ptr<asd::IndexBuffer_Imp>	indexBuffer;

		std::shared_ptr<Texture2D>				brdfTexture;
		
		Vector2DF Hammersley(uint32_t ind, uint32_t Count);
		float Frac(float v);

		float GGX_Smith1(float roughness, float dot);
		float GGX_Smith(float roughness, float NoV, float NoL);
		Vector3DF ImportanceSampleGGX(Vector2DF Xi, float Roughness, Vector3DF N);
		Vector2DF IntegrateBRDF(float Roughness, float NoV);

		void CalcIntegratedGGX_WithoutF0(int32_t width, int32_t height, std::vector<Color>& dst);

	public:
		EnvironmentRendering(Graphics* graphics, std::shared_ptr<asd::VertexBuffer_Imp> vb, std::shared_ptr<asd::IndexBuffer_Imp> ib);
		virtual ~EnvironmentRendering();

		void Render(RenderedCameraObject3DProxy* cameraP, RenderingCommandHelper* helper, RenderTexture2D_Imp* gb0, RenderTexture2D_Imp* gb1, RenderTexture2D_Imp* gb2, RenderTexture2D_Imp* gb3,
			float diffuseColorIntensity, float specularColorIntensity, CubemapTexture* diffuseColor, CubemapTexture* specularColor);
	};
}