
#include "ace.EnvironmentRendering.h"

#include "../../ace.Graphics_Imp.h"

#include "../../Resource/ace.ShaderCache.h"
#include "../../Resource/ace.CubemapTexture.h"

#include "../../Shader/DX/3D/Screen_VS.h"
#include "../../Shader/GL/3D/Screen_VS.h"

#include "../../Shader/DX/3D/Environment_PS.h"
#include "../../Shader/GL/3D/Environment_PS.h"

#include "../../Command/ace.RenderingCommandHelper.h"

#include "../Object/ace.RenderedCameraObject3D.h"

#include "ace.BRDF.h"

#include <fstream>

namespace ace
{
	/**
	参考　http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
	C++11化 and ACEの規約に変更
	*/
	Vector2DF EnvironmentRendering::Hammersley(uint32_t ind, uint32_t Count)
	{
		auto radicalInverse_VdC = [](uint32_t bits) -> float {
			bits = (bits << 16u) | (bits >> 16u);
			bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
			bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
			bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
			bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
			return float(bits) * 2.3283064365386963e-10; // / 0x100000000
		};

		return Vector2DF(float(ind) / float(Count), radicalInverse_VdC(ind));
	}

	float EnvironmentRendering::Frac(float v)
	{
		auto v_ = (int32_t) v;
		return v - v_;
	}

	float EnvironmentRendering::GGX_Smith1(float roughness, float dot)
	{
		float a2 = roughness * roughness * roughness * roughness;
		return 2.0f / (dot + sqrt(dot * (dot - dot * a2) + a2));
	}

	float EnvironmentRendering::GGX_Smith(float roughness, float NoV, float NoL)
	{
		return GGX_Smith1(roughness, NoV) * GGX_Smith1(roughness, NoL);
	}

	/**
		参考　http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
	*/
	Vector3DF EnvironmentRendering::ImportanceSampleGGX(Vector2DF Xi, float Roughness, Vector3DF N)
	{
		float a = Roughness * Roughness;
		float Phi = 2 * PI * Xi.X;
		float CosTheta = sqrt((1 - Xi.Y) / (1 + (a*a - 1) * Xi.Y));
		float SinTheta = sqrt(1 - CosTheta * CosTheta);
		Vector3DF H;
		H.X = SinTheta * cos(Phi);
		H.Y = SinTheta * sin(Phi);
		H.Z = CosTheta;
		Vector3DF UpVector = abs(N.Z) < 0.999 ? Vector3DF(0, 0, 1) : Vector3DF(1, 0, 0);
		Vector3DF TangentX = (Vector3DF::Cross(UpVector, N)).GetNormal();
		Vector3DF TangentY = Vector3DF::Cross(N, TangentX);
		// Tangent to world space
		return TangentX * H.X + TangentY * H.Y + N * H.Z;
	}

	/**
		参考　http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
	*/
	Vector2DF EnvironmentRendering::IntegrateBRDF(float Roughness, float NoV)
	{
		Vector3DF V;
		V.X = sqrt(1.0f - NoV * NoV); // sin
		V.Y = 0;
		V.Z = NoV; // cos

		Vector3DF N(0, 0, 1);
		float A = 0;
		float B = 0;
		const uint32_t NumSamples = 128;

		int32_t r1 = rand();
		int32_t r2 = rand();

		for (uint32_t i = 0; i < NumSamples; i++)
		{
			Vector2DF Xi = Hammersley(i, NumSamples);

			Vector3DF H = ImportanceSampleGGX(Xi, Roughness, N);
			Vector3DF L = H * 2 * Vector3DF::Dot(V, H) - V;
			float NoL = Clamp(L.Z, 1.0f, 0.0f);
			float NoH = Clamp(H.Z, 1.0f, 0.0f);
			float VoH = Clamp(Vector3DF::Dot(V, H), 1.0f, 0.0f);
			if (NoL > 0)
			{
				float G = GGX_Smith(Roughness, NoV, NoL);
				//float G_Vis = G * VoH / (NoH * NoV) / 4.0;	// /4.0はSmithに*4を加えた補正
				float G_Vis = G * VoH * (NoL / NoH);

				float Fc = pow(1 - VoH, 5);
				A += (1 - Fc) * G_Vis;
				B += Fc * G_Vis;
			}
		}
		return Vector2DF(A, B) / NumSamples;
	}

	void EnvironmentRendering::CalcIntegratedGGX_WithoutF0(int32_t width, int32_t height, std::vector<Color>& dst)
	{
		dst.resize(width * height);

		for (auto y = 0; y < height; y++)
		{
			for (auto x = 0; x < width; x++)
			{
				auto x_ = (float) x / (float) width + 0.5f / (float) width;
				auto y_ = (float) y / (float) height + 0.5f / (float) height;

				float v1 = 0;
				float v2 = 0;

				auto ret = IntegrateBRDF(y_, x_);
				v1 = ret.X;
				v2 = ret.Y;
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
		
		std::vector<Color> brdfTextureBuf;
#if 0
		// 外部出力
		CalcIntegratedGGX_WithoutF0(128, 128, brdfTextureBuf);

		ImageHelper::SavePNGImage(L"brdf.png", 128, 128, brdfTextureBuf.data(), false);

		std::ofstream ofs("brdf.h");

		ofs << "static const uint8_t g_brdf[] = {";
		for (int32_t y = 0; y < 128; y++)
		{
			for (int32_t x = 0; x < 128; x++)
			{
				ofs << (int) brdfTextureBuf[x + y * 128].R << ", " << (int) brdfTextureBuf[x + y * 128].G << ", " << (int) brdfTextureBuf[x + y * 128].B << ", " << (int) brdfTextureBuf[x + y * 128].A << ",";
			}
			ofs << std::endl;
		}
		ofs << "0x00 };";
		ofs.close();

#else
		brdfTextureBuf.resize(128 * 128);
		memcpy(brdfTextureBuf.data(), g_brdf, 128 * 128 * 4);
#endif
		brdfTexture = g->CreateEmptyTexture2D(128, 128, TextureFormat::R8G8B8A8_UNORM);
		TextureLockInfomation info;
		if (brdfTexture->Lock(info))
		{
			memcpy(info.Pixels, brdfTextureBuf.data(), 128 * 128 * 4);
			brdfTexture->Unlock();
		}
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
			h::GenValue("mipmapCount", (float)specularColor->GetMipmapCount()),
			h::GenValue("g_diffuseTexture", h::CubemapTexturePair(diffuseColor, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
			h::GenValue("g_specularTexture", h::CubemapTexturePair(specularColor, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
			h::GenValue("g_brdfTexture", h::Texture2DPair(brdfTexture.get(), ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
			h::GenValue("g_gbuffer0Texture", h::Texture2DPair(gb0, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
			h::GenValue("g_gbuffer1Texture", h::Texture2DPair(gb1, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
			h::GenValue("g_gbuffer2Texture", h::Texture2DPair(gb2, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp)),
			h::GenValue("g_gbuffer3Texture", h::Texture2DPair(gb3, ace::TextureFilterType::Linear, ace::TextureWrapType::Clamp))
			);
	}
}