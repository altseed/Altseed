
#include "ace.PostEffectLightBloom.h"
#include "../../ace.Engine.h"

namespace ace{


	PostEffectLightBloom::PostEffectLightBloom()
	{
		auto g = Engine::GetGraphics();
		std::string baseCode = m_corePostEffect->GetLightBloomShader(g->GetGraphicsDeviceType());

		std::string code = std::string("#define COPY 1\n") + baseCode;
		std::string codeX = std::string("#define BLUR_X 1\n") + baseCode;
		std::string codeY = std::string("#define BLUR_Y 1\n") + baseCode;
		std::string codeY_Sum = std::string("#define BLUR_Y_SUM 1\n") + baseCode;
		std::string codeSum = std::string("#define SUM 1\n") + baseCode;

		auto shader_copy = g->CreateShader2D(ace::ToAString(code.c_str()).c_str());
		material_copy = g->CreateMaterial2D(shader_copy);

		auto shaderX = g->CreateShader2D(ace::ToAString(codeX.c_str()).c_str());
		material2dX = g->CreateMaterial2D(shaderX);

		auto shaderY = g->CreateShader2D(ace::ToAString(codeY.c_str()).c_str());
		material2dY = g->CreateMaterial2D(shaderY);

		auto shaderY_Sum = g->CreateShader2D(ace::ToAString(codeY_Sum.c_str()).c_str());
		material2dY_Sum = g->CreateMaterial2D(shaderY_Sum);

		auto shaderSum = g->CreateShader2D(ace::ToAString(codeSum.c_str()).c_str());
		materialSum = g->CreateMaterial2D(shaderSum);

		std::string downsampleCode = m_corePostEffect->GetDownsampleShader(g->GetGraphicsDeviceType());
		auto downsampleShader = g->CreateShader2D(ace::ToAString(downsampleCode.c_str()).c_str());
		downsample = g->CreateMaterial2D(downsampleShader);
	}

	void PostEffectLightBloom::OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src)
	{

#if defined(DOWNSAMPLE)
		assert(src != nullptr);
		assert(dst != nullptr);

		Vector4DF weights1, weights2;
		float ws[8];
		float total = 0.0f;
		float const dispersion = intensity * intensity;
		for (int32_t i = 0; i < 8; i++)
		{
			float pos = 1.0f + 2.0f * i;
			ws[i] = expf(-0.5f * pos * pos / dispersion);
			total += ws[i] * 2.0f;
		}
		weights1.X = ws[0] / total;
		weights1.Y = ws[1] / total;
		weights1.Z = ws[2] / total;
		weights1.W = ws[3] / total;

		weights2.X = ws[4] / total;
		weights2.Y = ws[5] / total;
		weights2.Z = ws[6] / total;
		weights2.W = ws[7] / total;

		auto size = src->GetSize();
		auto format = src->GetFormat();

		if (tempTexture0 == nullptr ||
			(tempTexture0->GetSize() != size / 2 || tempTexture0->GetFormat() != format))
		{
			if (format ==TextureFormat::R32G32B32A32_FLOAT)
			{
				tempTexture0 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 2, size.Y / 2, TextureFormat::R32G32B32A32_FLOAT);
				tempTexture1 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 4, size.Y / 4, TextureFormat::R32G32B32A32_FLOAT);
				tempTexture2 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 8, size.Y / 8, TextureFormat::R32G32B32A32_FLOAT);

				downsampledTexture0 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 2, size.Y / 2, TextureFormat::R32G32B32A32_FLOAT);
				downsampledTexture1 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 4, size.Y / 4, TextureFormat::R32G32B32A32_FLOAT);
				downsampledTexture2 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 8, size.Y / 8, TextureFormat::R32G32B32A32_FLOAT);

				tempTexture2 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 8, size.Y / 8, TextureFormat::R8G8B8A8_UNORM);
				downsampledTexture2 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 8, size.Y / 8, TextureFormat::R8G8B8A8_UNORM);
			}
			else
			{
				tempTexture0 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 2, size.Y / 2, TextureFormat::R8G8B8A8_UNORM);
				tempTexture1 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 4, size.Y / 4, TextureFormat::R8G8B8A8_UNORM);
				tempTexture2 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 8, size.Y / 8, TextureFormat::R8G8B8A8_UNORM);

				downsampledTexture0 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 2, size.Y / 2, TextureFormat::R8G8B8A8_UNORM);
				downsampledTexture1 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 4, size.Y / 4, TextureFormat::R8G8B8A8_UNORM);
				downsampledTexture2 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 8, size.Y / 8, TextureFormat::R8G8B8A8_UNORM);
			}
		}

		downsample->SetTexture2D(ace::ToAString("g_texture").c_str(), src);
		downsample->SetTextureFilterType(ace::ToAString("g_texture").c_str(), TextureFilterType::Linear);
		downsample->SetVector2DF(ace::ToAString("g_offset").c_str(), Vector2DF(1.0f / (float) size.X, 1.0f / (float) size.Y));
		DrawOnTexture2DWithMaterial(downsampledTexture0, downsample);

		downsample->SetTexture2D(ace::ToAString("g_texture").c_str(), downsampledTexture0);
		downsample->SetTextureFilterType(ace::ToAString("g_texture").c_str(), TextureFilterType::Linear);
		downsample->SetVector2DF(ace::ToAString("g_offset").c_str(), Vector2DF(2.0f / (float) size.X, 2.0f / (float) size.Y));
		DrawOnTexture2DWithMaterial(downsampledTexture1, downsample);

		downsample->SetTexture2D(ace::ToAString("g_texture").c_str(), downsampledTexture1);
		downsample->SetTextureFilterType(ace::ToAString("g_texture").c_str(), TextureFilterType::Linear);
		downsample->SetVector2DF(ace::ToAString("g_offset").c_str(), Vector2DF(4.0f / (float) size.X, 4.0f / (float) size.Y));
		DrawOnTexture2DWithMaterial(downsampledTexture2, downsample);

		//material->SetTexture2D(ace::ToAString("g_originalTexture").c_str(), downsampledTexture);
		//DrawOnTexture2DWithMaterial(copiedTexture, material);

		material2dX->SetTexture2D(ace::ToAString("g_blurredTexture").c_str(), downsampledTexture2);
		material2dX->SetVector4DF(ace::ToAString("g_weight1").c_str(), weights1);
		material2dX->SetVector4DF(ace::ToAString("g_weight2").c_str(), weights2);
		material2dX->SetFloat(ace::ToAString("g_threshold").c_str(), threshold);
		material2dX->SetFloat(ace::ToAString("g_power").c_str(), power);
		material2dX->SetTextureFilterType(ace::ToAString("g_blurredTexture").c_str(), TextureFilterType::Linear);

		DrawOnTexture2DWithMaterial(tempTexture2, material2dX);

		material2dY->SetTexture2D(ace::ToAString("g_blurredTexture").c_str(), tempTexture2);
		material2dY->SetVector4DF(ace::ToAString("g_weight1").c_str(), weights1);
		material2dY->SetVector4DF(ace::ToAString("g_weight2").c_str(), weights2);
		material2dY->SetFloat(ace::ToAString("g_threshold").c_str(), threshold);
		material2dY->SetFloat(ace::ToAString("g_power").c_str(), power);
		material2dY->SetTextureFilterType(ace::ToAString("g_blurredTexture").c_str(), TextureFilterType::Linear);

		DrawOnTexture2DWithMaterial(downsampledTexture2, material2dY);

		materialSum->SetTexture2D(ace::ToAString("g_blurredTexture").c_str(), downsampledTexture2);
		materialSum->SetTexture2D(ace::ToAString("g_originalTexture").c_str(), src);
		materialSum->SetVector4DF(ace::ToAString("g_weight1").c_str(), weights1);
		materialSum->SetVector4DF(ace::ToAString("g_weight2").c_str(), weights2);
		materialSum->SetFloat(ace::ToAString("g_threshold").c_str(), threshold);
		materialSum->SetFloat(ace::ToAString("g_power").c_str(), power);
		materialSum->SetTextureFilterType(ace::ToAString("g_blurredTexture").c_str(), TextureFilterType::Linear);
		materialSum->SetTextureFilterType(ace::ToAString("g_originalTexture").c_str(), TextureFilterType::Linear);

		DrawOnTexture2DWithMaterial(dst, materialSum);
#else
		assert(src != nullptr);
		assert(dst != nullptr);

		Vector4DF weights1, weights2;
		float ws[8];
		float total = 0.0f;
		float const dispersion = intensity * intensity;
		for (int32_t i = 0; i < 8; i++)
		{
			float pos = 1.0f + 2.0f * i;
			ws[i] = expf(-0.5f * pos * pos / dispersion);
			total += ws[i] * 2.0f;
		}
		weights1.X = ws[0] / total;
		weights1.Y = ws[1] / total;
		weights1.Z = ws[2] / total;
		weights1.W = ws[3] / total;

		weights2.X = ws[4] / total;
		weights2.Y = ws[5] / total;
		weights2.Z = ws[6] / total;
		weights2.W = ws[7] / total;

		auto size = src->GetSize();
		auto format = src->GetFormat();

		if (tempTexture == nullptr ||
			(tempTexture->GetSize() != size || tempTexture->GetFormat() != format))
		{
			if (format ==TextureFormat::R32G32B32A32_FLOAT)
			{
				tempTexture = Engine::GetGraphics()->CreateRenderTexture2D(size.X, size.Y, TextureFormat::R32G32B32A32_FLOAT);
			}
			else
			{
				tempTexture = Engine::GetGraphics()->CreateRenderTexture2D(size.X, size.Y, TextureFormat::R8G8B8A8_UNORM);
			}
		}

		if (copiedTexture == nullptr ||
			(copiedTexture->GetSize() != size || copiedTexture->GetFormat() != format))
		{
			if (format == TextureFormat::R32G32B32A32_FLOAT)
			{
				copiedTexture = Engine::GetGraphics()->CreateRenderTexture2D(size.X, size.Y, TextureFormat::R32G32B32A32_FLOAT);
			}
			else
			{
				copiedTexture = Engine::GetGraphics()->CreateRenderTexture2D(size.X, size.Y, TextureFormat::R8G8B8A8_UNORM);
			}
		}

		material_copy->SetTexture2D(ace::ToAString("g_originalTexture").c_str(), src);
		DrawOnTexture2DWithMaterial(copiedTexture, material_copy);
		
		material2dX->SetTexture2D(ace::ToAString("g_blurredTexture").c_str(), src);
		material2dX->SetVector4DF(ace::ToAString("g_weight1").c_str(), weights1);
		material2dX->SetVector4DF(ace::ToAString("g_weight2").c_str(), weights2);
		material2dX->SetFloat(ace::ToAString("g_threshold").c_str(), threshold);
		material2dX->SetFloat(ace::ToAString("g_power").c_str(), power);
		material2dX->SetTextureFilterType(ace::ToAString("g_blurredTexture").c_str(), TextureFilterType::Linear);

		DrawOnTexture2DWithMaterial(tempTexture, material2dX);

		material2dY_Sum->SetTexture2D(ace::ToAString("g_blurredTexture").c_str(), tempTexture);
		material2dY_Sum->SetTexture2D(ace::ToAString("g_originalTexture").c_str(), copiedTexture);
		material2dY_Sum->SetVector4DF(ace::ToAString("g_weight1").c_str(), weights1);
		material2dY_Sum->SetVector4DF(ace::ToAString("g_weight2").c_str(), weights2);
		material2dY_Sum->SetFloat(ace::ToAString("g_threshold").c_str(), threshold);
		material2dY_Sum->SetFloat(ace::ToAString("g_power").c_str(), power);
		material2dY_Sum->SetTextureFilterType(ace::ToAString("g_blurredTexture").c_str(), TextureFilterType::Linear);
		material2dY_Sum->SetTextureFilterType(ace::ToAString("g_originalTexture").c_str(), TextureFilterType::Linear);

		DrawOnTexture2DWithMaterial(dst, material2dY_Sum);
#endif
	}

}