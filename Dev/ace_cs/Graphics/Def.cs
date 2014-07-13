using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 描画時のブレンドモードを表す列挙体
	/// </summary>
	public enum AlphaBlendMode
	{
		/// <summary>
		/// 不透明
		/// </summary>
		Opacity = swig.AlphaBlend.Opacity,
		/// <summary>
		/// 透明
		/// </summary>
		Blend = swig.AlphaBlend.Blend,
		/// <summary>
		/// 加算
		/// </summary>
		Add = swig.AlphaBlend.Add,
		/// <summary>
		/// 減算
		/// </summary>
		Sub = swig.AlphaBlend.Sub,
		/// <summary>
		/// 乗算
		/// </summary>
		Mul = swig.AlphaBlend.Mul,
	}

	public enum TextureFormat
	{
		R8G8B8A8_UNORM = swig.eTextureFormat.TEXTURE_FORMAT_R8G8B8A8_UNORM,
		R32G32B32A32_FLOAT = swig.eTextureFormat.TEXTURE_FORMAT_R32G32B32A32_FLOAT,
		R8G8B8A8_UNORM_SRGB = swig.eTextureFormat.TEXTURE_FORMAT_R8G8B8A8_UNORM_SRGB,
		R16G16_FLOAT = swig.eTextureFormat.TEXTURE_FORMAT_GL_R16G16_FLOAT,
	}

	/// <summary>
	/// 描画方法
	/// </summary>
	public enum GraphicsDeviceType
	{
		/// <summary>
		/// 実行環境で最も安定している描画方法(初期化時に使用)
		/// </summary>
		Default = swig.GraphicsDeviceType.Default,
		/// <summary>
		/// DirectX11
		/// </summary>
		DirectX11 = swig.GraphicsDeviceType.DirectX11,
		/// <summary>
		/// OpenGL
		/// </summary>
		OpenGL = swig.GraphicsDeviceType.OpenGL,
	}

    public enum TextureFilterType
    {
		Nearest = swig.TextureFilterType.Nearest,
		Linear = swig.TextureFilterType.Linear,
    }

	public enum TextureWrapType
	{
		Clamp = swig.TextureWrapType.Clamp,
		Repeat = swig.TextureWrapType.Repeat,
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
	/// 3D描画時に表示されるバッファ
	/// </summary>
	public enum VisualizedBuffer : int 
	{
		FinalImage = swig.eVisalizedBuffer.VISALIZED_BUFFER_FINALIMAGE,
		Diffuse = swig.eVisalizedBuffer.VISALIZED_BUFFER_DIFFUSE,
		Normal = swig.eVisalizedBuffer.VISALIZED_BUFFER_NORMAL,
	}

    ///<summary>
    /// テキストの描画方向
    /// </summary>
    public enum WritingDirection :int
    {
        Horizontal = swig.WritingDirection.Horizontal,
        Vertical = swig.WritingDirection.Vertical,
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

	/// <summary>
	/// 描画設定のクラス
	/// </summary>
	public class RenderSettings
	{
		/// <summary>
		/// 遅延レンダリングから軽量レンダリングに変更し高速に描画するか?
		/// </summary>
		public bool IsLightweightMode = false;

		/// <summary>
		/// 画面に表示されるバッファ
		/// </summary>
		public VisualizedBuffer VisualizedBuffer = VisualizedBuffer.FinalImage;

		public RenderSettings()
		{
		}

		internal RenderSettings(swig.RenderSettings settings)
		{
			IsLightweightMode = settings.IsLightweightMode;
			VisualizedBuffer = (VisualizedBuffer)settings.VisalizedBuffer;
		}
	}
}
