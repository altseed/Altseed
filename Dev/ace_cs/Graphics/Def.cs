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
		R8G8B8A8_UNORM = swig.TextureFormat.R8G8B8A8_UNORM,
		R32G32B32A32_FLOAT = swig.TextureFormat.R32G32B32A32_FLOAT,
		R8G8B8A8_UNORM_SRGB = swig.TextureFormat.R8G8B8A8_UNORM_SRGB,
		R16G16_FLOAT = swig.TextureFormat.R16G16_FLOAT,
		R8_UNORM = swig.TextureFormat.R8_UNORM,
		BC1 = swig.TextureFormat.BC1,
		BC1_SRGB = swig.TextureFormat.BC1_SRGB,
		BC2 = swig.TextureFormat.BC2,
		BC2_SRGB = swig.TextureFormat.BC2_SRGB,
		BC3 = swig.TextureFormat.BC3,
		BC3_SRGB = swig.TextureFormat.BC3_SRGB,
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
		Quaternion = swig.RotationOrder.QUATERNION,
		XZY  = swig.RotationOrder.XZY,
		XYZ  = swig.RotationOrder.XYZ,
		ZXY  = swig.RotationOrder.ZXY,
		ZYX  = swig.RotationOrder.ZYX,
		YXZ  = swig.RotationOrder.YXZ,
		YZX  = swig.RotationOrder.YZX,
		Axis = swig.RotationOrder.AXIS,
	}

	public enum InterpolationType : int
	{
		Constant = swig.InterpolationType.Constant,
		Linear = swig.InterpolationType.Linear,
		Cubic = swig.InterpolationType.Cubic,
	}

	/// <summary>
	/// 3D描画時に表示されるバッファ
	/// </summary>
	public enum VisualizedBufferType : int 
	{
		FinalImage = swig.VisualizedBufferType.FinalImage,
		DiffuseColor = swig.VisualizedBufferType.DiffuseColor,
		SpecularColor = swig.VisualizedBufferType.SpecularColor,
		Smoothness = swig.VisualizedBufferType.Smoothness,
		Normal = swig.VisualizedBufferType.Normal,
		Environment = swig.VisualizedBufferType.Environment,
		Occulusion = swig.VisualizedBufferType.Occlusion,
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

	public class TextureLockInfomation
	{
		public IntPtr Pixels
		{
			get
			{
				return SwigObject.GetPixels();
			}
		}

		public int Pitch
		{
			get
			{
				return SwigObject.GetPitch();
			}
		}

		public Vector2DI Size
		{
			get
			{
				return SwigObject.GetSize();
			}
		}

		internal swig.TextureLockInfomation SwigObject = null;

		public TextureLockInfomation()
		{
			SwigObject = new swig.TextureLockInfomation();
		}
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
		public VisualizedBufferType VisualizedBuffer = VisualizedBufferType.FinalImage;

		public RenderSettings()
		{
		}

		internal RenderSettings(swig.RenderSettings settings)
		{
			IsLightweightMode = settings.IsLightweightMode;
			VisualizedBuffer = (VisualizedBufferType)settings.VisualizedBuffer;
		}
	}
}
