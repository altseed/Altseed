using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 輝度の高い画素のみをぼかして加算するポストエフェクト
	/// </summary>
	public class PostEffectLightBloom : PostEffect
	{
		Material2D material2dX_Lum, material2dX, material2dY, materialSum, downsample;

		RenderTexture2D tempTexture0;
		RenderTexture2D tempTexture1;
		RenderTexture2D tempTexture2;
		RenderTexture2D tempTexture3;

		RenderTexture2D downsampledTexture0;
		RenderTexture2D downsampledTexture1;
		RenderTexture2D downsampledTexture2;
		RenderTexture2D downsampledTexture3;

		float intensity = 5.0f;
		float threshold = 1.0f;
		float exposure = 1.0f;
		bool isLuminanceMode = false;

		/// <summary>
		/// ぼかしの強さを設定する。
		/// </summary>
		/// <remarks>
		/// 実質的にはガウス関数の分散にあたる値の平方根
		/// </remarks>
		public float Intensity
		{
			set
			{
				if (value <= 0.0f) return;
				intensity = value;
			}
		}

		/// <summary>
		/// ぼかすピクセルの閾値を設定する。
		/// </summary>
		/// <remarks>
		/// この閾値を超えた値がぼかされ加算される。
		/// </remarks>
		public float Threshold
		{
			set { threshold = value; }
		}

		/// <summary>
		/// 露光の強さを指定する。
		/// </summary>
		/// <remarks>
		/// この値が高いほどぼかされる値の輝度が高くなる。
		/// </remarks>
		public float Exposure
		{
			set { exposure = value; }
		}

		/// <summary>
		/// 輝度を参照するか指定する。
		/// </summary>
		/// <param name="value"></param>
		/// <remarks>
		/// この値がtrueだと、RGBではなく輝度からぼかされる色を決定する。
		/// </remarks>
		public bool IsLuminanceMode
		{
			set
			{
				isLuminanceMode = value;
			}
		}

		public PostEffectLightBloom()
		{
			var g = Engine.Graphics;
			var baseCode = SwigObject.GetLightBloomShader((swig.GraphicsDeviceType)g.GraphicsDeviceType);

			var codeX = "#define BLUR_X 1\n" + baseCode;
			var codeX_Lum = "#define BLUR_X 1\n#define LUM 1\n" + baseCode;
			var codeY = "#define BLUR_Y 1\n" + baseCode;
			var codeSum = "#define SUM 1\n" + baseCode;

			var shaderX = g.CreateShader2D(codeX);
			material2dX = g.CreateMaterial2D(shaderX);

			var shaderX_Lum = g.CreateShader2D(codeX_Lum);
			material2dX_Lum = g.CreateMaterial2D(shaderX_Lum);

			var shaderY = g.CreateShader2D(codeY);
			material2dY = g.CreateMaterial2D(shaderY);

			var shaderSum = g.CreateShader2D(codeSum);
			materialSum = g.CreateMaterial2D(shaderSum);

			string downsampleCode = SwigObject.GetDownsampleShader((swig.GraphicsDeviceType)g.GraphicsDeviceType);
			var downsampleShader = g.CreateShader2D(downsampleCode);
			downsample = g.CreateMaterial2D(downsampleShader);
		}

		public override void OnDraw(RenderTexture2D dst, RenderTexture2D src)
		{

			Vector4DF weights1 = new Vector4DF();
			Vector4DF weights2 = new Vector4DF();

			float[] ws = new float[8];
			float total = 0.0f;
			float dispersion = intensity * intensity;
			for (int i = 0; i < 8; i++)
			{
				float pos = 1.0f + 2.0f * i;
				ws[i] = (float)Math.Exp(-0.5f * pos * pos / dispersion);
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

			var size = src.Size;
			var format = src.Format;

			if (tempTexture0 == null ||
				(tempTexture0.Size != size / 2 || tempTexture0.Format != format))
			{
				if (format == TextureFormat.R32G32B32A32_FLOAT)
				{
					tempTexture0 = Engine.Graphics.CreateRenderTexture2D(size.X / 2, size.Y / 2, TextureFormat.R32G32B32A32_FLOAT);
					tempTexture1 = Engine.Graphics.CreateRenderTexture2D(size.X / 4, size.Y / 4, TextureFormat.R32G32B32A32_FLOAT);
					tempTexture2 = Engine.Graphics.CreateRenderTexture2D(size.X / 8, size.Y / 8, TextureFormat.R32G32B32A32_FLOAT);
					tempTexture3 = Engine.Graphics.CreateRenderTexture2D(size.X / 16, size.Y / 16, TextureFormat.R32G32B32A32_FLOAT);

					downsampledTexture0 = Engine.Graphics.CreateRenderTexture2D(size.X / 2, size.Y / 2, TextureFormat.R32G32B32A32_FLOAT);
					downsampledTexture1 = Engine.Graphics.CreateRenderTexture2D(size.X / 4, size.Y / 4, TextureFormat.R32G32B32A32_FLOAT);
					downsampledTexture2 = Engine.Graphics.CreateRenderTexture2D(size.X / 8, size.Y / 8, TextureFormat.R32G32B32A32_FLOAT);
					downsampledTexture3 = Engine.Graphics.CreateRenderTexture2D(size.X / 16, size.Y / 16, TextureFormat.R32G32B32A32_FLOAT);
				}
				else
				{
					tempTexture0 = Engine.Graphics.CreateRenderTexture2D(size.X / 2, size.Y / 2, TextureFormat.R8G8B8A8_UNORM);
					tempTexture1 = Engine.Graphics.CreateRenderTexture2D(size.X / 4, size.Y / 4, TextureFormat.R8G8B8A8_UNORM);
					tempTexture2 = Engine.Graphics.CreateRenderTexture2D(size.X / 8, size.Y / 8, TextureFormat.R8G8B8A8_UNORM);
					tempTexture3 = Engine.Graphics.CreateRenderTexture2D(size.X / 16, size.Y / 16, TextureFormat.R8G8B8A8_UNORM);

					downsampledTexture0 = Engine.Graphics.CreateRenderTexture2D(size.X / 2, size.Y / 2, TextureFormat.R8G8B8A8_UNORM);
					downsampledTexture1 = Engine.Graphics.CreateRenderTexture2D(size.X / 4, size.Y / 4, TextureFormat.R8G8B8A8_UNORM);
					downsampledTexture2 = Engine.Graphics.CreateRenderTexture2D(size.X / 8, size.Y / 8, TextureFormat.R8G8B8A8_UNORM);
					downsampledTexture3 = Engine.Graphics.CreateRenderTexture2D(size.X / 16, size.Y / 16, TextureFormat.R8G8B8A8_UNORM);
				}
			}

			downsample.SetTexture2D("g_texture", src);
			downsample.SetTextureFilterType("g_texture", TextureFilterType.Linear);
			downsample.SetVector2DF("g_offset", new Vector2DF(1.0f / (float)size.X, 1.0f / (float)size.Y));
			DrawOnTexture2DWithMaterial(downsampledTexture0, downsample);

			downsample.SetTexture2D("g_texture", downsampledTexture0);
			downsample.SetTextureFilterType("g_texture", TextureFilterType.Linear);
			downsample.SetVector2DF("g_offset", new Vector2DF(2.0f / (float)size.X, 2.0f / (float)size.Y));
			DrawOnTexture2DWithMaterial(downsampledTexture1, downsample);

			downsample.SetTexture2D("g_texture", downsampledTexture1);
			downsample.SetTextureFilterType("g_texture", TextureFilterType.Linear);
			downsample.SetVector2DF("g_offset", new Vector2DF(4.0f / (float)size.X, 4.0f / (float)size.Y));
			DrawOnTexture2DWithMaterial(downsampledTexture2, downsample);

			downsample.SetTexture2D("g_texture", downsampledTexture2);
			downsample.SetTextureFilterType("g_texture", TextureFilterType.Linear);
			downsample.SetVector2DF("g_offset", new Vector2DF(8.0f / (float)size.X, 8.0f / (float)size.Y));
			DrawOnTexture2DWithMaterial(downsampledTexture3, downsample);

			ace.Material2D blurX = null;
			if (isLuminanceMode)
			{
				blurX = material2dX_Lum;
			}
			else
			{
				blurX = material2dX;
			}

			// ブラー1
			blurX.SetTexture2D("g_blurredTexture", downsampledTexture1);
			blurX.SetVector4DF("g_weight1", weights1);
			blurX.SetVector4DF("g_weight2", weights2);
			blurX.SetFloat("g_threshold", threshold);
			blurX.SetFloat("g_exposure", exposure);
			blurX.SetTextureFilterType("g_blurredTexture", TextureFilterType.Linear);
			DrawOnTexture2DWithMaterial(tempTexture1, blurX);

			material2dY.SetTexture2D("g_blurredTexture", tempTexture1);
			material2dY.SetVector4DF("g_weight1", weights1);
			material2dY.SetVector4DF("g_weight2", weights2);
			material2dY.SetTextureFilterType("g_blurredTexture", TextureFilterType.Linear);
			DrawOnTexture2DWithMaterial(downsampledTexture1, material2dY);

			// ブラー2
			blurX.SetTexture2D("g_blurredTexture", downsampledTexture2);
			blurX.SetVector4DF("g_weight1", weights1);
			blurX.SetVector4DF("g_weight2", weights2);
			blurX.SetFloat("g_threshold", threshold);
			blurX.SetFloat("g_exposure", exposure);
			blurX.SetTextureFilterType("g_blurredTexture", TextureFilterType.Linear);
			DrawOnTexture2DWithMaterial(tempTexture2, blurX);

			material2dY.SetTexture2D("g_blurredTexture", tempTexture2);
			material2dY.SetVector4DF("g_weight1", weights1);
			material2dY.SetVector4DF("g_weight2", weights2);
			material2dY.SetTextureFilterType("g_blurredTexture", TextureFilterType.Linear);
			DrawOnTexture2DWithMaterial(downsampledTexture2, material2dY);

			// ブラー3
			blurX.SetTexture2D("g_blurredTexture", downsampledTexture3);
			blurX.SetVector4DF("g_weight1", weights1);
			blurX.SetVector4DF("g_weight2", weights2);
			blurX.SetFloat("g_threshold", threshold);
			blurX.SetFloat("g_exposure", exposure);
			blurX.SetTextureFilterType("g_blurredTexture", TextureFilterType.Linear);
			DrawOnTexture2DWithMaterial(tempTexture3, blurX);

			material2dY.SetTexture2D("g_blurredTexture", tempTexture3);
			material2dY.SetVector4DF("g_weight1", weights1);
			material2dY.SetVector4DF("g_weight2", weights2);
			material2dY.SetTextureFilterType("g_blurredTexture", TextureFilterType.Linear);
			DrawOnTexture2DWithMaterial(downsampledTexture3, material2dY);

			// 合計
			materialSum.SetTexture2D("g_blurred0Texture", downsampledTexture1);
			materialSum.SetTexture2D("g_blurred1Texture", downsampledTexture2);
			materialSum.SetTexture2D("g_blurred2Texture", downsampledTexture3);
			materialSum.SetTexture2D("g_originalTexture", src);
			materialSum.SetTextureFilterType("g_blurred0Texture", TextureFilterType.Linear);
			materialSum.SetTextureFilterType("g_blurred1Texture", TextureFilterType.Linear);
			materialSum.SetTextureFilterType("g_blurred2Texture", TextureFilterType.Linear);
			materialSum.SetTextureFilterType("g_originalTexture", TextureFilterType.Linear);

			DrawOnTexture2DWithMaterial(dst, materialSum);
		}
	}
}
