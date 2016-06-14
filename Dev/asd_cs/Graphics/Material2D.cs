using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	public partial class Material2D : IReleasable
	{
		internal Material2D(swig.Material2D instance)
		{
			CoreInstance = instance;
		}

		~Material2D()
		{
			ForceToRelease();
		}

		public bool IsReleased
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
		public void ForceToRelease()
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
