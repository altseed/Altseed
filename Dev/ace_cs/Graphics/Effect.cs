using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// エフェクトの情報が記録されているクラス
	/// </summary>
	public class Effect : IDestroy
	{
		internal swig.Effect SwigObject { get; set; }

		internal Effect(swig.Effect swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.Effects.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			SwigObject = swig;
		}

		~Effect()
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
	}
}
