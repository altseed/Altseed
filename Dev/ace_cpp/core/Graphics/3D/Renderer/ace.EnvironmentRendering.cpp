
#include "ace.EnvironmentRendering.h"

#include "../../ace.Graphics_Imp.h"
#include "../../Resource/ace.ShaderCache.h"

#include "../../Shader/DX/3D/Screen_VS.h"
#include "../../Shader/GL/3D/Screen_VS.h"

#include "../../Shader/DX/3D/Environment_PS.h"
#include "../../Shader/GL/3D/Environment_PS.h"

#include "../../Command/ace.RenderingCommandHelper.h"

#include "../Object/ace.RenderedCameraObject3D.h"

namespace ace
{
	EnvironmentRendering::EnvironmentRendering(Graphics* graphics, std::shared_ptr<ace::VertexBuffer_Imp> vb, std::shared_ptr<ace::IndexBuffer_Imp> ib)
	{
		auto g = (Graphics_Imp*) graphics;
		vertexBuffer = vb;
		indexBuffer = ib;

		std::vector<ace::VertexLayout> vl;
		vl.push_back(ace::VertexLayout("Position", ace::LAYOUT_FORMAT_R32G32B32_FLOAT));
		vl.push_back(ace::VertexLayout("UV", ace::LAYOUT_FORMAT_R32G32_FLOAT));

		std::vector<ace::Macro> macro;
	
		if (g->GetGraphicsDeviceType() == GraphicsDeviceType::OpenGL)
		{
			environmentShader = g->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.Environment").c_str(),
				screen_vs_gl,
				environment_ps_gl,
				vl,
				macro);
		}
		else
		{
			environmentShader = g->GetShaderCache()->CreateFromCode(
				ToAString(L"Internal.Environment").c_str(),
				screen_vs_dx,
				environment_ps_dx,
				vl,
				macro);
		}
	}

	EnvironmentRendering::~EnvironmentRendering()
	{
	
	}

	void EnvironmentRendering::Render(RenderedCameraObject3DProxy* cameraP, RenderingCommandHelper* helper, RenderTexture2D_Imp* gb0, RenderTexture2D_Imp* gb1, RenderTexture2D_Imp* gb2, RenderTexture2D_Imp* gb3,
		CubemapTexture* diffuseColor, CubemapTexture* specularColor)
	{
		if (environmentShader.get() == nullptr) return;

		using h = RenderingCommandHelper;

		helper->SetRenderTarget(cameraP->GetRenderTargetEnvironment(), nullptr);
		helper->Clear(true, false, Color(0, 0, 0, 255));

		RenderState state;
		state.DepthTest = false;
		state.DepthWrite = false;
		state.AlphaBlendState = AlphaBlend::Opacity;
		state.Culling = ace::CullingType::Double;

		helper->Draw(2, vertexBuffer.get(), indexBuffer.get(), environmentShader.get(), state,
			h::GenValue("upDir", cameraP->UpDir),
			h::GenValue("rightDir", cameraP->RightDir),
			h::GenValue("frontDir", cameraP->FrontDir),
			h::GenValue("reconstructInfo1", cameraP->ReconstructInfo1),
			h::GenValue("reconstructInfo2", cameraP->ReconstructInfo2),
			h::GenValue("g_diffuseTexture", h::CubemapTexturePair(diffuseColor, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
			h::GenValue("g_specularTexture", h::CubemapTexturePair(specularColor, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
			h::GenValue("g_gbuffer0Texture", h::Texture2DPair(gb0, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
			h::GenValue("g_gbuffer1Texture", h::Texture2DPair(gb1, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
			h::GenValue("g_gbuffer2Texture", h::Texture2DPair(gb2, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
			h::GenValue("g_gbuffer3Texture", h::Texture2DPair(gb3, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp))
			);
	}
}