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
	public partial class SoundSource : IDestroy
	{
		internal SoundSource(swig.SoundSource swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.SoundSources.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			CoreInstance = swig;
		}

		~SoundSource()
		{
			Destroy();
		}

		public bool IsDestroyed
		{
			get
			{
				return CoreInstance == null;
			}
		}

		public void Destroy()
		{
			lock (this)
			{
				if (CoreInstance == null) return;
				GC.Collector.AddObject(CoreInstance);
				CoreInstance = null;
			}
			Particular.GC.SuppressFinalize(this);
		}
	}
}
