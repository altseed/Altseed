using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 音源のクラス
	/// </summary>
	public class SoundSource : IDestroy
	{
		internal swig.SoundSource SwigObject;

		internal SoundSource(swig.SoundSource swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.SoundSources.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			SwigObject = swig;
		}

		~SoundSource()
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
			System.GC.SuppressFinalize(this);
		}

		/// <summary>
		/// ループポイントの開始地点(秒)を取得、または設定する。
		/// </summary>
		public float LoopStartingPoint
		{
			get { return SwigObject.GetLoopStartingPoint(); }
			set { SwigObject.SetLoopEndPoint(value); }
		}

		/// <summary>
		/// ループポイントの終了地点(秒)を取得、または設定する。
		/// </summary>
		public float LoopEndPoint
		{
			get { return SwigObject.GetLoopEndPoint(); }
			set { SwigObject.SetLoopEndPoint(value); }
		}

		/// <summary>
		/// ループするかを取得、または設定する。
		/// </summary>
		public bool IsLoopingMode
		{
			get { return SwigObject.GetIsLoopingMode(); }
			set { SwigObject.SetIsLoopingMode(value); }
		}

		/// <summary>
		/// 音の長さを取得する。
		/// </summary>
		public float Length
		{
			get { return SwigObject.GetLength(); }
		}
	}
}
