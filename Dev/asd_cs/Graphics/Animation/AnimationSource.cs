using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// アニメーションの元データのクラス
	/// </summary>
	public class AnimationSource : IReleasable
	{
		internal swig.AnimationSource SwigObject { get; set; }

		internal AnimationSource(swig.AnimationSource swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.AnimationSources.GetObject(swig.GetPtr()) != null) Particular.Helper.ThrowException("");
#endif
			SwigObject = swig;
		}

		~AnimationSource()
		{
			ForceToRelease();
		}

		public bool IsReleased
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
		public void ForceToRelease()
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
		/// アニメーションの長さ(60フレーム単位)を取得する。
		/// </summary>
		public float Length
		{
			get { return SwigObject.GetLength(); }
		}

		/// <summary>
		/// ボーンごとのアニメーションを追加する。
		/// </summary>
		/// <param name="keyframeAnimation">アニメーション</param>
		public void AddAnimation(KeyframeAnimation keyframeAnimation) 
		{
			SwigObject.AddAnimation(keyframeAnimation.SwigObject);
		}

		/// <summary>
		/// ボーンごとのアニメーションを削除する。
		/// </summary>
		/// <param name="keyframeAnimation">アニメーション</param>
		public void RemoveAnimation(KeyframeAnimation keyframeAnimation) 
		{
			SwigObject.RemoveAnimation(keyframeAnimation.SwigObject);
		}
	}
}
