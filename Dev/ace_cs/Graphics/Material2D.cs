using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class Material2D : IDestroy
	{
		internal swig.Material2D SwigObject { get; set; }

		internal Material2D(swig.Material2D swig)
		{
			SwigObject = swig;
		}

		~Material2D()
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

		public float GetFloat(string name)
		{
			return SwigObject.GetFloat(name);
		}

		public void SetFloat(string name, float value)
		{
			SwigObject.SetFloat(name, value);
		}

		public Vector2DF GetVector2DF(string name)
		{
			return SwigObject.GetVector2DF(name);
		}

		public void SetVector2DF(string name, Vector2DF value)
		{
			SwigObject.SetVector2DF(name, value);
		}

		public Vector3DF GetVector3DF(string name)
		{
			return SwigObject.GetVector3DF(name);
		}

		public void SetVector3DF(string name, Vector3DF value)
		{
			SwigObject.SetVector3DF(name, value);
		}

		public Vector4DF GetVector4DF(string name)
		{
			return SwigObject.GetVector4DF(name);
		}

		public void SetVector4DF(string name, Vector4DF value)
		{
			SwigObject.SetVector4DF(name, value);
		}

		public Texture2D GetTexture2D(string name)
		{
			var swigObj = swig.Accessor.Material2D_GetTexture2D(SwigObject, name);

			var existing = GC.Texture2Ds.GetObject(swigObj.GetPtr());
			if (existing != null) return existing;

			swigObj.AddRef();
			return new Texture2D(swigObj);
		}

		public void SetTexture2D(string name, Texture2D value)
		{
			if (value == null) return;
			SwigObject.SetTexture2D(name, IG.GetTexture2D(value));
		}

		public TextureFilterType GetTextureFilterType(string name)
		{
			return (TextureFilterType)SwigObject.GetTextureFilterType(name); 
		}

		public void SetTextureFilterType(string name, TextureFilterType filter)
		{
			SwigObject.SetTextureFilterType(name, (swig.TextureFilterType)filter);
		}

		public TextureWrapType GetTextureWrapType(string name)
		{
			return (TextureWrapType)SwigObject.GetTextureWrapType(name);
		}

		public void SetTextureWrapType(string name, TextureWrapType wrap)
		{
			SwigObject.SetTextureWrapType(name, (swig.TextureWrapType)wrap);
		}

		public Shader2D GetShader2D(string name)
		{
			var swigObj = swig.Accessor.Material2D_GetShader2D(SwigObject);

			var existing = GC.Shader2Ds.GetObject(swigObj.GetPtr());
			if (existing != null) return existing;

			swigObj.AddRef();
			return new Shader2D(swigObj);
		}

		public void SetShader2D(string name, Shader2D value)
		{
			SwigObject.SetShader2D(value.SwigObject);
		}
	}
}
