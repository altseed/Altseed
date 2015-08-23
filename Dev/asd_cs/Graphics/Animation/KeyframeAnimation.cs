using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// キーフレームで指定するアニメーションのクラス
	/// </summary>
	public class KeyframeAnimation : IDestroy
	{
		internal swig.KeyframeAnimation SwigObject { get; set; }

		internal KeyframeAnimation(swig.KeyframeAnimation swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.KeyframeAnimations.GetObject(swig.GetPtr()) != null) Particular.Helper.ThrowException("");
#endif
			SwigObject = swig;
		}

		~KeyframeAnimation()
		{
			Destroy();
		}

		public bool IsDestroyed
		{
			get
			{
				return SwigObject == null;
			}
		}

		/// <summary>
		/// 強制的に使用しているメモリを開放する。
		/// </summary>
		/// <remarks>
		/// 何らかの理由でメモリが不足した場合に実行する。
		/// 開放した後の動作の保証はしていないので、必ず参照が残っていないことを確認する必要がある。
		/// </remarks>
		public void Destroy()
		{
			lock (this)
			{
				if (SwigObject == null) return;
				GC.Collector.AddObject(SwigObject);
				SwigObject = null;
			}
			Particular.GC.SuppressFinalize(this);
		}

		/// <summary>
		/// 名称を取得、または設定する。
		/// </summary>
		public string Name
		{
			get
			{
				return SwigObject.GetName();
			}
			set
			{
				SwigObject.SetName(value);
			}
		}

		/// <summary>
		/// キーフレームを追加する。
		/// </summary>
		/// <param name="kf">キーフレーム</param>
		public void AddKeyframe(FCurveKeyframe kf)
		{
			SwigObject.AddKeyframe(ref kf);
		}

		/// <summary>
		/// 指定した時間の値を取得する。
		/// </summary>
		/// <param name="time">時間</param>
		/// <returns>値</returns>
		public float GetValue(float time)
		{
			return SwigObject.GetValue(time);
		}
	}
}
