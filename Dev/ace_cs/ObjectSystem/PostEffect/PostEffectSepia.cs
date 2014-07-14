using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// セピア調化するポストエフェクトクラス
	/// </summary>
	public class PostEffectSepia : PostEffect
	{
		Material2D material2d;

		public PostEffectSepia()
		{
			var g = Engine.Graphics;
			var shader = g.CreateShader2D(SwigObject.GetSepiaShader((swig.GraphicsDeviceType)g.GraphicsDeviceType));
			material2d = g.CreateMaterial2D(shader);
		}

		public override void OnDraw(RenderTexture2D dst, RenderTexture2D src)
		{
			material2d.SetTexture2D("g_texture", src);
			DrawOnTexture2DWithMaterial(dst, material2d);
		}
	}
}
