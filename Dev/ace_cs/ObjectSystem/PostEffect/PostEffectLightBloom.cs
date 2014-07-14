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
		Material2D material, material2dX, material2dY;
		RenderTexture2D tempTexture;
		RenderTexture2D copiedTexture;

		float intensity = 5.0f;
		float threshold = 1.0f;
		float power = 1.0f;

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
		/// ぼかされた値を加算する強さを指定する。
		/// </summary>
		/// <remarks>
		/// この値とぼかされた値を乗算した値が加算される。
		/// </remarks>
		public float Power
		{
			set { power = value; }
		}

		public PostEffectLightBloom()
		{
			var g = Engine.Graphics;
			var baseCode = SwigObject.GetLightBloomShader((swig.GraphicsDeviceType)g.GraphicsDeviceType);

			var code = "#define COPY 1\n" + baseCode;
			var codeX = "#define BLUR_X 1\n" + baseCode;
			var codeY = "#define BLUR_Y 1\n" + baseCode;

			var shader = g.CreateShader2D(code);
			material = g.CreateMaterial2D(shader);

			var shaderX = g.CreateShader2D(codeX);
			material2dX = g.CreateMaterial2D(shaderX);

			var shaderY = g.CreateShader2D(codeY);
			material2dY = g.CreateMaterial2D(shaderY);
		}

		public override void OnDraw(RenderTexture2D dst, RenderTexture2D src)
		{

			Vector4DF weights;
			float[] ws = new float[4];
			float total = 0.0f;
			float dispersion = intensity * intensity;
			for (int i = 0; i < 4; i++)
			{
				float pos = 1.0f + 2.0f * i;
				ws[i] = (float)Math.Exp(-0.5f * pos * pos / dispersion);
				total += ws[i] * 2.0f;
			}
			weights.X = ws[0] / total;
			weights.Y = ws[1] / total;
			weights.Z = ws[2] / total;
			weights.W = ws[3] / total;

			var size = src.Size;
			var format = src.Format;

			if (tempTexture == null ||
				(tempTexture.Size != size || tempTexture.Format != format))
			{
				if (format == TextureFormat.R32G32B32A32_FLOAT)
				{
					tempTexture = Engine.Graphics.CreateRenderTexture2D(size.X, size.Y, TextureFormat.R32G32B32A32_FLOAT);
				}
				else
				{
					tempTexture = Engine.Graphics.CreateRenderTexture2D(size.X, size.Y, TextureFormat.R8G8B8A8_UNORM);
				}
			}

			if (copiedTexture == null ||
				(copiedTexture.Size != size || copiedTexture.Format != format))
			{
				if (format == TextureFormat.R32G32B32A32_FLOAT)
				{
					copiedTexture = Engine.Graphics.CreateRenderTexture2D(size.X, size.Y, TextureFormat.R32G32B32A32_FLOAT);
				}
				else
				{
					copiedTexture = Engine.Graphics.CreateRenderTexture2D(size.X, size.Y, TextureFormat.R8G8B8A8_UNORM);
				}
			}

			material.SetTexture2D("g_originalTexture", src);
			DrawOnTexture2DWithMaterial(copiedTexture, material);

			material2dX.SetTexture2D("g_blurredTexture", src);
			material2dX.SetVector4DF("g_weight", weights);
			material2dX.SetFloat("g_threshold", threshold);
			material2dX.SetFloat("g_power", power);
			material2dX.SetTextureFilterType("g_blurredTexture", TextureFilterType.Linear);

			DrawOnTexture2DWithMaterial(tempTexture, material2dX);

			material2dY.SetTexture2D("g_blurredTexture", tempTexture);
			material2dY.SetTexture2D("g_originalTexture", copiedTexture);
			material2dY.SetVector4DF("g_weight", weights);
			material2dY.SetFloat("g_threshold", threshold);
			material2dY.SetFloat("g_power", power);
			material2dY.SetTextureFilterType("g_blurredTexture", TextureFilterType.Linear);
			material2dY.SetTextureFilterType("g_originalTexture", TextureFilterType.Linear);

			DrawOnTexture2DWithMaterial(dst, material2dY);
		}
	}
}
