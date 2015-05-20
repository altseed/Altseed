using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 地形のクラス
	/// </summary>
	public partial class Terrain3D : IDestroy
	{
		internal Terrain3D(swig.Terrain3D swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.Terrain3Ds.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			CoreInstance = swig;
		}

		~Terrain3D()
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
			System.GC.SuppressFinalize(this);
		}

		/// <summary>
		/// 材質を設定する。
		/// </summary>
		/// <param name="material">材質</param>
		public void SetMaterial(int materialIndex, Material3D material)
		{
			CoreInstance.SetMaterial(IG.GetMaterial3D(material));
		}
	}
}
