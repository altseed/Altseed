using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// キューブマップ
	/// </summary>
	public class CubemapTexture : Texture, IDestroy
	{
		internal swig.CubemapTexture SwigObject { get; set; }

		internal CubemapTexture(swig.CubemapTexture swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if(GC.CubemapTextures.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			SwigObject = swig;
		}

		~CubemapTexture()
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

		/// <summary>
		/// ミップマップ数
		/// </summary>
		public int MipmapCount
		{
			get
			{
				return SwigObject.GetMipmapCount();
			}
		}
	}
}
