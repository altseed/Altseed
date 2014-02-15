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
		R8G8B8A8_UNORM = swig.eTextureFormat.TEXTURE_FORMAT_R8G8B8A8_UNORM,
		R32G32B32A32_FLOAT = swig.eTextureFormat.TEXTURE_FORMAT_R32G32B32A32_FLOAT
	}

	public enum GraphicsType
	{
		DirectX11 = swig.eGraphicsType.GRAPHICS_TYPE_DX11,
		OpenGL = swig.eGraphicsType.GRAPHICS_TYPE_GL,
	}

    public enum TextureFilterType
    {
		Nearest = swig.eTextureFilterType.TEXTURE_FILTER_NEAREST,
		Linear = swig.eTextureFilterType.TEXTURE_FILTER_LINEAR,
    }

	public enum RotationOrder
	{
		Quaternion = swig.eRotationOrder.ROTATION_ORDER_QUATERNION,
		XZY  = swig.eRotationOrder.ROTATION_ORDER_XZY,
		XYZ  = swig.eRotationOrder.ROTATION_ORDER_XYZ,
		ZXY  = swig.eRotationOrder.ROTATION_ORDER_ZXY,
		ZYX  = swig.eRotationOrder.ROTATION_ORDER_ZYX,
		YXZ  = swig.eRotationOrder.ROTATION_ORDER_YXZ,
		YZX  = swig.eRotationOrder.ROTATION_ORDER_YZX,
		Axis = swig.eRotationOrder.ROTATION_ORDER_AXIS,
	}

	public enum InterpolationType : int
	{
		Constant = swig.eInterpolationType.INTERPOLATION_TYPE_CONSTANT,
		Linear = swig.eInterpolationType.INTERPOLATION_TYPE_LINEAR,
		Cubic = swig.eInterpolationType.INTERPOLATION_TYPE_CUBIC,
	}

	/// <summary>
	/// Fカーブのキーフレーム
	/// </summary>
	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
	public struct FCurveKeyframe
	{
		/// <summary>
		/// 時間と値
		/// </summary>
		public Vector2DF KeyValue;

		/// <summary>
		/// 制御用ハンドル
		/// </summary>
		public Vector2DF LeftHandle;

		/// <summary>
		/// 制御用ハンドル
		/// </summary>
		public Vector2DF RightHandle;

		/// <summary>
		/// 補間方法
		/// </summary>
		public InterpolationType InterpolationType;

		public FCurveKeyframe(InterpolationType interpolationType = ace.InterpolationType.Linear)
		{
			LeftHandle = new Vector2DF();
			RightHandle = new Vector2DF();
			KeyValue = new Vector2DF();
			InterpolationType = interpolationType;
		}
	}

	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
	public struct TextureLockInfomation
	{
		public IntPtr Pixels;
		public int Pitch;
		public Vector2DI Size;

		/// <summary>
		/// ロックしたテクスチャの参照
		/// C++側に影響がないよう、最後尾に配置
		/// </summary>
		internal Texture2D texture;
	}
}
