using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
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
		public InterpolationType Interpolation;

		public FCurveKeyframe(InterpolationType interpolationType = asd.InterpolationType.Linear)
		{
			LeftHandle = new Vector2DF();
			RightHandle = new Vector2DF();
			KeyValue = new Vector2DF();
			Interpolation = interpolationType;
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
