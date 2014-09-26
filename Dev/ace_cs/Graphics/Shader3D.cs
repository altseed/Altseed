using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class Shader3D : IDestroy
	{
		internal swig.Shader3D SwigObject { get; set; }

		internal Shader3D(swig.Shader3D swig)
		{
			SwigObject = swig;
		}

		~Shader3D()
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
