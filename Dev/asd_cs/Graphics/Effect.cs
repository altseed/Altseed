using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// エフェクトの情報が記録されているクラス
	/// </summary>
	public partial class Effect : IDestroy
	{
		internal Effect(swig.Effect swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.Effects.GetObject(swig.GetPtr()) != null) Particular.Helper.ThrowException("");
#endif
			CoreInstance = swig;
		}

		~Effect()
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
