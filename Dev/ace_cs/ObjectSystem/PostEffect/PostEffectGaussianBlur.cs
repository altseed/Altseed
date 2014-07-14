using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// ガウスぼかしをかけるポストエフェクトクラス
	/// </summary>
	public class PostEffectGaussianBlur : PostEffect
	{
		Material2D material2dX;
		Material2D material2dY;
		RenderTexture2D tempTexture;
		float intensity = 5.0f;

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

		public PostEffectGaussianBlur()
		{
			var g = Engine.Graphics;
			var baseShader = SwigObject.GetGaussianBlurShader((swig.GraphicsDeviceType)g.GraphicsDeviceType);
			var codeX = "#define BLUR_X 1\n" + baseShader;
			var codeY = "#define BLUR_Y 1\n" + baseShader;

			var shaderX = g.CreateShader2D(codeX);
			material2dX = g.CreateMaterial2D(shaderX);

			var shaderY = g.CreateShader2D(codeY);
			material2dY = g.CreateMaterial2D(shaderY);
		}

		public override void OnDraw(RenderTexture2D dst, RenderTexture2D src)
		{
			Vector3DF weights;
			float[] ws = new float[3];
			float total = 0.0f;
			float dispersion = intensity * intensity;
			for (int i = 0; i < 3; i++)
			{
				float pos = 1.0f + 2.0f * i;
				ws[i] = (float)Math.Exp(-0.5f * pos * pos / dispersion);
				total += ws[i] * 2.0f;
			}
			weights.X = ws[0] / total;
			weights.Y = ws[1] / total;
			weights.Z = ws[2] / total;

			material2dX.SetTexture2D("g_texture", src);
			material2dX.SetVector3DF("g_weight", weights);
			material2dX.SetTextureFilterType("g_texture", TextureFilterType.Linear);

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

			DrawOnTexture2DWithMaterial(tempTexture, material2dX);

			material2dY.SetTexture2D("g_texture", tempTexture);
			material2dY.SetVector3DF("g_weight", weights);
			material2dY.SetTextureFilterType("g_texture", TextureFilterType.Linear);

			DrawOnTexture2DWithMaterial(dst, material2dY);
		}

	}
}
