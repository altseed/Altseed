using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class Model : IDestroy
	{
		internal swig.Model SwigObject { get; set; }

		internal Model(swig.Model swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.Models.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			SwigObject = swig;
		}

		~Model()
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
