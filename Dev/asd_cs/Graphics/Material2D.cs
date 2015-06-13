using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	public partial class Material2D : IDestroy
	{
		internal Material2D(swig.Material2D swig)
		{
			CoreInstance = swig;
		}

		~Material2D()
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

		public Texture2D GetTexture2D(string name)
		{
			var swigObj = swig.Accessor.Material2D_GetTexture2D(CoreInstance, name);

			var existing = GC.Texture2Ds.GetObject(swigObj.GetPtr());
			if (existing != null) return existing;

			swigObj.AddRef();
			return new Texture2D(swigObj);
		}

		public Shader2D GetShader2D(string name)
		{
			var swigObj = swig.Accessor.Material2D_GetShader2D(CoreInstance);

			var existing = GC.Shader2Ds.GetObject(swigObj.GetPtr());
			if (existing != null) return existing;

			swigObj.AddRef();
			return new Shader2D(swigObj);
		}

		public void SetShader2D(string name, Shader2D value)
		{
			CoreInstance.SetShader2D(value.CoreInstance);
		}
	}
}
