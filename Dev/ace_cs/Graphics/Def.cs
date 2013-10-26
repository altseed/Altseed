using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 描画の際のブレンドモードを表す列挙体。
	/// </summary>
	public enum AlphaBlendMode
	{
		Opacity = swig.eAlphaBlend.ALPHA_BLEND_OPACITY,
		Blend = swig.eAlphaBlend.ALPHA_BLEND_BLEND,
		/// <summary>
		/// 加算合成。
		/// </summary>
		Add = swig.eAlphaBlend.ALPHA_BLEND_ADD,
		/// <summary>
		/// 減算合成。
		/// </summary>
		Sub = swig.eAlphaBlend.ALPHA_BLEND_SUB,
		/// <summary>
		/// 乗算合成。
		/// </summary>
		Mul = swig.eAlphaBlend.ALPHA_BLEND_MUL,
		Dword = swig.eAlphaBlend.ALPHA_BLEND_DWORD,
	}

	public enum TextureFormat
	{
		RGBA8888 = swig.eTextureFormat.TEXTURE_FORMAT_RGBA8888
	}

	public enum GraphicsType
	{
		DirectX11 = swig.eGraphicsType.GRAPHICS_TYPE_DX11,
		OpenGL = swig.eGraphicsType.GRAPHICS_TYPE_GL,
	}
}
