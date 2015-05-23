using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public partial class Material3D : IDestroy
	{
		internal Material3D(swig.Material3D swig)
		{
			CoreInstance = swig;
		}

		~Material3D()
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
			var swigObj = swig.Accessor.Material3D_GetTexture2D(CoreInstance, name);

			var existing = GC.Texture2Ds.GetObject(swigObj.GetPtr());
			if (existing != null) return existing;

			swigObj.AddRef();
			return new Texture2D(swigObj);
		}

		public Shader3D GetShader3D(string name)
		{
			var swigObj = swig.Accessor.Material3D_GetShader3D(CoreInstance);

			var existing = GC.Shader3Ds.GetObject(swigObj.GetPtr());
			if (existing != null) return existing;

			swigObj.AddRef();
			return new Shader3D(swigObj);
		}

		public void SetShader2D(string name, Shader3D value)
		{
			CoreInstance.SetShader3D(value.CoreInstance);
		}
	}
}
