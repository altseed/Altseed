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
		internal ace.swig.Terrain3D CoreInstance { get { return coreInstance; } }

		internal Terrain3D(swig.Terrain3D swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.Terrain3Ds.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			coreInstance = swig;
		}

		~Terrain3D()
		{
			Destroy();
		}

		public bool IsDestroyed
		{
			get
			{
				return coreInstance == null;
			}
		}

		public void Destroy()
		{
			lock (this)
			{
				if (coreInstance == null) return;
				GC.Collector.AddObject(coreInstance);
				coreInstance = null;
			}
			System.GC.SuppressFinalize(this);
		}

		/// <summary>
		/// 材質を設定する。
		/// </summary>
		/// <param name="material">材質</param>
		public void SetMaterial(int materialIndex, Material3D material)
		{
			coreInstance.SetMaterial(IG.GetMaterial3D(material));
		}
	}
}
