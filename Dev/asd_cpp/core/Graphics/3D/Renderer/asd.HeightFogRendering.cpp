
#include "asd.HeightFogRendering.h"

#include "../../asd.Graphics_Imp.h"

#include "../../Resource/asd.ShaderCache.h"

#include "../../Shader/DX/3D/Screen_VS.h"
#include "../../Shader/GL/3D/Screen_VS.h"

#include "../../Shader/DX/3D/HeightFog_PS.h"
//#include "../../Shader/GL/3D/HeighFog_PS.h"

#include "../../Command/asd.RenderingCommandHelper.h"

#include "../Object/asd.RenderedCameraObject3D.h"

#include "asd.BRDF.h"

#include <fstream>

namespace asd
{
	HeightFogRendering::HeightFogRendering(Graphics* graphics, std::shared_ptr<asd::VertexBuffer_Imp> vb, std::shared_ptr<asd::IndexBuffer_Imp> ib)
	{
		auto g = (Graphics_Imp*) graphics;
		vertexBuffer = vb;
		indexBuffer = ib;

		std::vector<asd::VertexLayout> vl;
		vl.push_back(asd::VertexLayout("Position", asd::VertexLayoutFormat::R32G32B32_FLOAT));
		vl.push_back(asd::VertexLayout("UV", asd::VertexLayoutFormat::R32G32_FLOAT));

		std::vector<asd::Macro> macro;

		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			//environmentShader = g->GetShaderCache()->CreateFromCode(
			//	ToAString(L"Internal.Environment").c_str(),
			//	screen_vs_gl,
			//	environment_ps_gl,
			//	vl,
			//	macro);
		}
		else
		{
			heightFogShader = g->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.HeightFog").c_str(),
				screen_vs_dx,
				heightfog_ps_dx,
				vl,
				macro);
		}
	}

	HeightFogRendering::~HeightFogRendering()
	{

	}

	void HeightFogRendering::Render(RenderedCameraObject3DProxy* cameraP, RenderingCommandHelper* helper, RenderTexture2D_Imp* gb0, RenderTexture2D_Imp* gb1, RenderTexture2D_Imp* gb2, RenderTexture2D_Imp* gb3)
	{
		if (heightFogShader == nullptr) return;
		using h = RenderingCommandHelper;

		RenderState state;
		state.DepthTest = false;
		state.DepthWrite = false;
		state.AlphaBlendState = AlphaBlendMode::Blend;
		state.Culling = asd::CullingType::Double;

		helper->SetRenderTarget(cameraP->GetRenderTarget(), nullptr);
		
		auto cameraMatInv = cameraP->CameraMatrix.GetInverted();

		Vector4DF fogColor = Vector4DF(FogColor.R / 255.0f, FogColor.G / 255.0f, FogColor.B / 255.0f, 1.0f);
		Vector4DF fogParam0;
		Vector4DF fogParam1;

		fogParam0.X = StartDistance;
		fogParam0.Y = Density;
		fogParam0.Z = Falloff;

		helper->Draw(2, vertexBuffer.get(), indexBuffer.get(), heightFogShader.get(), state,
			h::GenValue("reconstructInfo1", cameraP->ReconstructInfo1),
			h::GenValue("reconstructInfo2", cameraP->ReconstructInfo2),
			h::GenValue("g_cameraPositionToWorldPosition", cameraMatInv),
			h::GenValue("g_fogColor", fogColor),
			h::GenValue("g_fogParam0", fogParam0),
			h::GenValue("g_fogParam1", fogParam1),
			h::GenValue("g_gbuffer0Texture", h::Texture2DPair(gb0, asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp)),
			h::GenValue("g_gbuffer1Texture", h::Texture2DPair(gb1, asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp)),
			h::GenValue("g_gbuffer2Texture", h::Texture2DPair(gb2, asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp)),
			h::GenValue("g_gbuffer3Texture", h::Texture2DPair(gb3, asd::TextureFilterType::Linear, asd::TextureWrapType::Clamp))
			);
	}
}