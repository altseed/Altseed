using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class Shader2D : IDestroy
		{
		internal swig.Shader2D SwigObject { get; set; }

		internal Shader2D(swig.Shader2D swig)
		{
			SwigObject = swig;
		}

		~Shader2D()
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
	}
}
