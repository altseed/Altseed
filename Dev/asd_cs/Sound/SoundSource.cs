using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
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
			if (GC.SoundSources.GetObject(swig.GetPtr()) != null) Particular.Helper.ThrowException("");
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
				if (CoreInstance == null) return;
				GC.Collector.AddObject(CoreInstance);
				CoreInstance = null;
			}
			Particular.GC.SuppressFinalize(this);
		}
	}
}
