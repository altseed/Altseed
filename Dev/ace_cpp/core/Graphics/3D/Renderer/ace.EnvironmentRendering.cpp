
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
	float EnvironmentRendering::G1V(float dotNV, float k)
	{
		return 1.0f / (dotNV*(1.0f - k) + k);
	}

	void EnvironmentRendering::CalcGGX_WithoutF0(const Vector3DF& normal, const Vector3DF& light, const Vector3DF& view, float roughness, float& v1, float& v2)
	{
		auto alpha = roughness*roughness;
		auto V = view;
		auto L = light;
		auto N = normal;

		auto H = (V + L).GetNormal();

		float dotNL = Clamp(Vector3DF::Dot(N, L), 1.0f, 0.0f);
		float dotNV = Clamp(Vector3DF::Dot(N, V), 1.0f, 0.0f);
		float dotNH = Clamp(Vector3DF::Dot(N, H), 1.0f, 0.0f);
		float dotLH = Clamp(Vector3DF::Dot(L, H), 1.0f, 0.0f);

		float F, D, vis;

		// D
		float alphaSqr = alpha*alpha;
		float pi = 3.14159f;
		float denom = dotNH * dotNH *(alphaSqr - 1.0) + 1.0f;
		D = alphaSqr / (pi * denom * denom);

		// F
		float dotLH5 = pow(1.0f - dotLH, 5);

		// V
		float k = alpha / 2.0f;
		vis = G1V(dotNL, k)*G1V(dotNV, k);

		v1 = D * vis * (1 - dotLH5);
		v2 = D * vis * (dotLH5);
	}

	void EnvironmentRendering::IntegrateGGX_WithoutF0(float NoV, float roughness, float& v1, float& v2)
	{
		const int32_t sampleMax = 1024;
		
		roughness = (roughness + 1.0f) / 2.0f;

		v1 = 0.0f;
		v2 = 0.0f;

		Vector3DF V;
		V.X = sqrt(1.0f - NoV * NoV);
		V.Y = 0;
		V.Z = NoV;

		Vector3DF N(0.0f, 0.0f, 1.0f);

		for (auto i = 0; i < sampleMax; i++)
		{
			float a = roughness * roughness;

			auto xix = rand() / (float) RAND_MAX * 2.0f - 1.0f;
			auto xiy = rand() / (float) RAND_MAX * 2.0f - 1.0f;

			float Phi = 2 * 3.14f * xix;
			float CosTheta = sqrt((1 - xiy) / (1 + (a*a - 1) * xiy));
			float SinTheta = sqrt(1 - CosTheta * CosTheta);
			Vector3DF H;
			H.X = SinTheta * cos(Phi);
			H.Y = SinTheta * sin(Phi);
			H.Z = CosTheta;
			Vector3DF UpVector = abs(N.Z) < 0.999 ? Vector3DF(0, 0, 1) : Vector3DF(1, 0, 0);
			Vector3DF TangentX = Vector3DF::Cross(UpVector, N).GetNormal();
			Vector3DF TangentY = Vector3DF::Cross(N, TangentX);
			// Tangent to world space
			H = TangentX * H.X + TangentY * H.Y + N * H.Z;

			Vector3DF L = H * 2 * Vector3DF::Dot(V, H) - V;
		
			if (L.Z > 0.0f)
			{
				float v1_ = 0.0f;
				float v2_ = 0.0f;
				CalcGGX_WithoutF0(N, L, V, roughness, v1_, v2_);

				v1 += v1_;
				v2 += v2_;
			}
			
		}

		v1 /= (float)sampleMax;
		v2 /= (float)sampleMax;
	}

	void EnvironmentRendering::CalcIntegratedGGX_WithoutF0(int32_t width, int32_t height, std::vector<Color>& dst)
	{
		dst.resize(width * height);

		for (auto y = 1; y < height; y++)
		{
			for (auto x = 1; x < width; x++)
			{
				auto x_ = (float) x / (float) width;
				auto y_ = (float) y / (float) height;

				float v1 = 0;
				float v2 = 0;

				IntegrateGGX_WithoutF0(x_, y_, v1, v2);
				uint8_t v1__ = Clamp(v1 * 255, 255, 0);
				uint8_t v2__ = Clamp(v2 * 255, 255, 0);

				Color c;
				c.R = v1__;
				c.G = v2__;
				c.B = 0;
				c.A = 255;
				dst[x + y * width] = c;
			}
		}
	}

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

		//std::vector<Color> dst;
		//CalcIntegratedGGX_WithoutF0(32, 32, dst);

		//ImageHelper::SavePNGImage(L"test.png", 32, 32, dst.data(), false);
	}

	EnvironmentRendering::~EnvironmentRendering()
	{
	
	}

	void EnvironmentRendering::Render(RenderedCameraObject3DProxy* cameraP, RenderingCommandHelper* helper, RenderTexture2D_Imp* gb0, RenderTexture2D_Imp* gb1, RenderTexture2D_Imp* gb2, RenderTexture2D_Imp* gb3,
		CubemapTexture* diffuseColor, CubemapTexture* specularColor)
	{
		using h = RenderingCommandHelper;

		helper->SetRenderTarget(cameraP->GetRenderTargetEnvironment(), nullptr);
		helper->Clear(true, false, Color(0, 0, 0, 255));

		if (environmentShader.get() == nullptr) return;

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