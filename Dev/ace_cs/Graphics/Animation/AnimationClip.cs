using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// アニメーションソースを元に情報が付加されたアニメーションクラス
	/// </summary>
	public class AnimationClip : IDestroy
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
			Destroy();
		}

		public bool IsDestroyed
		{
			get
			{
				return SwigObject == null;
			}
		}

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
