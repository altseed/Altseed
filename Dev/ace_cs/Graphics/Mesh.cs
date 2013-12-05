using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class Mesh : IDestroy
	{
		internal swig.Mesh SwigObject { get; set; }

		internal Mesh(swig.Mesh swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if(GC.Meshs.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			SwigObject = swig;
		}

		~Mesh()
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
