
#include "asd.PostEffectLightBloom.h"
#include "../../asd.Engine.h"

namespace asd{


	PostEffectLightBloom::PostEffectLightBloom()
	{
		auto g = Engine::GetGraphics();
		std::string baseCode = m_corePostEffect->GetLightBloomShader(g->GetGraphicsDeviceType());

		std::string codeX = std::string("#define BLUR_X 1\n") + baseCode;
		std::string codeX_Lum = std::string("#define BLUR_X 1\n#define LUM 1\n") + baseCode;
		std::string codeY = std::string("#define BLUR_Y 1\n") + baseCode;
		std::string codeSum = std::string("#define SUM 1\n") + baseCode;

		auto shaderX = g->CreateShader2D(asd::ToAString(codeX.c_str()).c_str());
		material2dX = g->CreateMaterial2D(shaderX);

		auto shaderX_Lum = g->CreateShader2D(asd::ToAString(codeX_Lum.c_str()).c_str());
		material2dX_Lum = g->CreateMaterial2D(shaderX_Lum);

		auto shaderY = g->CreateShader2D(asd::ToAString(codeY.c_str()).c_str());
		material2dY = g->CreateMaterial2D(shaderY);

		auto shaderSum = g->CreateShader2D(asd::ToAString(codeSum.c_str()).c_str());
		materialSum = g->CreateMaterial2D(shaderSum);

		std::string downsampleCode = m_corePostEffect->GetDownsampleShader(g->GetGraphicsDeviceType());
		auto downsampleShader = g->CreateShader2D(asd::ToAString(downsampleCode.c_str()).c_str());
		downsample = g->CreateMaterial2D(downsampleShader);
	}

	void PostEffectLightBloom::OnDraw(std::shared_ptr<RenderTexture2D> dst, std::shared_ptr<RenderTexture2D> src)
	{
		assert(src != nullptr);
		assert(dst != nullptr);
		//return;
		Vector4DF weights1;
		Vector4DF weights2;
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
				tempTexture3 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 16, size.Y / 16, TextureFormat::R32G32B32A32_FLOAT);

				downsampledTexture0 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 2, size.Y / 2, TextureFormat::R32G32B32A32_FLOAT);
				downsampledTexture1 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 4, size.Y / 4, TextureFormat::R32G32B32A32_FLOAT);
				downsampledTexture2 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 8, size.Y / 8, TextureFormat::R32G32B32A32_FLOAT);
				downsampledTexture3 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 16, size.Y / 16, TextureFormat::R32G32B32A32_FLOAT);
			}
			else
			{
				tempTexture0 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 2, size.Y / 2, TextureFormat::R8G8B8A8_UNORM);
				tempTexture1 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 4, size.Y / 4, TextureFormat::R8G8B8A8_UNORM);
				tempTexture2 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 8, size.Y / 8, TextureFormat::R8G8B8A8_UNORM);
				tempTexture3 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 16, size.Y / 16, TextureFormat::R8G8B8A8_UNORM);

				downsampledTexture0 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 2, size.Y / 2, TextureFormat::R8G8B8A8_UNORM);
				downsampledTexture1 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 4, size.Y / 4, TextureFormat::R8G8B8A8_UNORM);
				downsampledTexture2 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 8, size.Y / 8, TextureFormat::R8G8B8A8_UNORM);
				downsampledTexture3 = Engine::GetGraphics()->CreateRenderTexture2D(size.X / 16, size.Y / 16, TextureFormat::R8G8B8A8_UNORM);
			}
		}

		downsample->SetTexture2D(asd::ToAString("g_texture").c_str(), src);
		downsample->SetTextureFilterType(asd::ToAString("g_texture").c_str(), TextureFilterType::Linear);
		downsample->SetVector2DF(asd::ToAString("g_offset").c_str(), Vector2DF(1.0f / (float) size.X, 1.0f / (float) size.Y));
		DrawOnTexture2DWithMaterial(downsampledTexture0, downsample);

		downsample->SetTexture2D(asd::ToAString("g_texture").c_str(), downsampledTexture0);
		downsample->SetTextureFilterType(asd::ToAString("g_texture").c_str(), TextureFilterType::Linear);
		downsample->SetVector2DF(asd::ToAString("g_offset").c_str(), Vector2DF(2.0f / (float) size.X, 2.0f / (float) size.Y));
		DrawOnTexture2DWithMaterial(downsampledTexture1, downsample);

		downsample->SetTexture2D(asd::ToAString("g_texture").c_str(), downsampledTexture1);
		downsample->SetTextureFilterType(asd::ToAString("g_texture").c_str(), TextureFilterType::Linear);
		downsample->SetVector2DF(asd::ToAString("g_offset").c_str(), Vector2DF(4.0f / (float) size.X, 4.0f / (float) size.Y));
		DrawOnTexture2DWithMaterial(downsampledTexture2, downsample);

		downsample->SetTexture2D(asd::ToAString("g_texture").c_str(), downsampledTexture2);
		downsample->SetTextureFilterType(asd::ToAString("g_texture").c_str(), TextureFilterType::Linear);
		downsample->SetVector2DF(asd::ToAString("g_offset").c_str(), Vector2DF(8.0f / (float) size.X, 8.0f / (float) size.Y));
		DrawOnTexture2DWithMaterial(downsampledTexture3, downsample);
		
		std::shared_ptr<asd::Material2D> blurX;
		if (isLuminanceMode)
		{
			blurX = material2dX_Lum;
		}
		else
		{
			blurX = material2dX;
		}


		// ブラー1
		blurX->SetTexture2D(asd::ToAString("g_blurredTexture").c_str(), downsampledTexture1);
		blurX->SetVector4DF(asd::ToAString("g_weight1").c_str(), weights1);
		blurX->SetVector4DF(asd::ToAString("g_weight2").c_str(), weights2);
		blurX->SetFloat(asd::ToAString("g_threshold").c_str(), threshold);
		blurX->SetFloat(asd::ToAString("g_exposure").c_str(), exposure);
		blurX->SetTextureFilterType(asd::ToAString("g_blurredTexture").c_str(), TextureFilterType::Linear);
		DrawOnTexture2DWithMaterial(tempTexture1, blurX);

		material2dY->SetTexture2D(asd::ToAString("g_blurredTexture").c_str(), tempTexture1);
		material2dY->SetVector4DF(asd::ToAString("g_weight1").c_str(), weights1);
		material2dY->SetVector4DF(asd::ToAString("g_weight2").c_str(), weights2);
		material2dY->SetTextureFilterType(asd::ToAString("g_blurredTexture").c_str(), TextureFilterType::Linear);
		DrawOnTexture2DWithMaterial(downsampledTexture1, material2dY);

		// ブラー2
		blurX->SetTexture2D(asd::ToAString("g_blurredTexture").c_str(), downsampledTexture2);
		blurX->SetVector4DF(asd::ToAString("g_weight1").c_str(), weights1);
		blurX->SetVector4DF(asd::ToAString("g_weight2").c_str(), weights2);
		blurX->SetFloat(asd::ToAString("g_threshold").c_str(), threshold);
		blurX->SetFloat(asd::ToAString("g_exposure").c_str(), exposure);
		blurX->SetTextureFilterType(asd::ToAString("g_blurredTexture").c_str(), TextureFilterType::Linear);
		DrawOnTexture2DWithMaterial(tempTexture2, blurX);

		material2dY->SetTexture2D(asd::ToAString("g_blurredTexture").c_str(), tempTexture2);
		material2dY->SetVector4DF(asd::ToAString("g_weight1").c_str(), weights1);
		material2dY->SetVector4DF(asd::ToAString("g_weight2").c_str(), weights2);
		material2dY->SetTextureFilterType(asd::ToAString("g_blurredTexture").c_str(), TextureFilterType::Linear);
		DrawOnTexture2DWithMaterial(downsampledTexture2, material2dY);

		// ブラー3
		blurX->SetTexture2D(asd::ToAString("g_blurredTexture").c_str(), downsampledTexture3);
		blurX->SetVector4DF(asd::ToAString("g_weight1").c_str(), weights1);
		blurX->SetVector4DF(asd::ToAString("g_weight2").c_str(), weights2);
		blurX->SetFloat(asd::ToAString("g_threshold").c_str(), threshold);
		blurX->SetFloat(asd::ToAString("g_exposure").c_str(), exposure);
		blurX->SetTextureFilterType(asd::ToAString("g_blurredTexture").c_str(), TextureFilterType::Linear);
		DrawOnTexture2DWithMaterial(tempTexture3, blurX);

		material2dY->SetTexture2D(asd::ToAString("g_blurredTexture").c_str(), tempTexture3);
		material2dY->SetVector4DF(asd::ToAString("g_weight1").c_str(), weights1);
		material2dY->SetVector4DF(asd::ToAString("g_weight2").c_str(), weights2);
		material2dY->SetTextureFilterType(asd::ToAString("g_blurredTexture").c_str(), TextureFilterType::Linear);
		DrawOnTexture2DWithMaterial(downsampledTexture3, material2dY);
		
		// 合計
		materialSum->SetTexture2D(asd::ToAString("g_blurred0Texture").c_str(), downsampledTexture1);
		materialSum->SetTexture2D(asd::ToAString("g_blurred1Texture").c_str(), downsampledTexture2);
		materialSum->SetTexture2D(asd::ToAString("g_blurred2Texture").c_str(), downsampledTexture3);
		materialSum->SetTexture2D(asd::ToAString("g_originalTexture").c_str(), src);
		materialSum->SetTextureFilterType(asd::ToAString("g_blurred0Texture").c_str(), TextureFilterType::Linear);
		materialSum->SetTextureFilterType(asd::ToAString("g_blurred1Texture").c_str(), TextureFilterType::Linear);
		materialSum->SetTextureFilterType(asd::ToAString("g_blurred2Texture").c_str(), TextureFilterType::Linear);
		materialSum->SetTextureFilterType(asd::ToAString("g_originalTexture").c_str(), TextureFilterType::Linear);

		DrawOnTexture2DWithMaterial(dst, materialSum);
	}

}