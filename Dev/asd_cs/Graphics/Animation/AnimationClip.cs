using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// アニメーションソースを元に情報が付加されたアニメーションクラス
	/// </summary>
	public class AnimationClip : IReleasable
	{
		internal swig.AnimationClip SwigObject { get; set; }

		internal AnimationClip(swig.AnimationClip swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.AnimationClips.GetObject(swig.GetPtr()) != null) Particular.Helper.ThrowException("");
#endif
			SwigObject = swig;
		}

		~AnimationClip()
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
		/// ソースを取得、または設定する。
		/// </summary>
		public AnimationSource Source
		{
			get
			{
				return GC.GenerateAnimationSource(swig.Accessor.AnimationClip_GetSource(SwigObject), GC.GenerationType.Get);
			}
			set
			{
				SwigObject.SetSource(value.SwigObject);
			}
		}

		/// <summary>
		/// ループするかを取得、または設定する。
		/// </summary>
		public bool IsLoopingMode
		{
			get { return SwigObject.GetIsLoopingMode(); }
			set { SwigObject.SetIsLoopingMode(value); }
		}
	}
}
