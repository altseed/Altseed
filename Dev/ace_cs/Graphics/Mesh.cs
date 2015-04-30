using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 3Dメッシュのクラス
	/// </summary>
	public partial class Mesh : IDestroy
	{
		internal Mesh(swig.Mesh swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.Meshs.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			CoreInstance = swig;
		}

		~Mesh()
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
		/// <param name="materialIndex">材質のインデックス</param>
		/// <param name="material">材質</param>
		/// <remarks>AddMaterialCountを実行した後でないと無効になる。</remarks>
		public void SetMaterial(int materialIndex, Material3D material)
		{
			CoreInstance.SetMaterial(materialIndex, IG.GetMaterial3D(material));
		}
	}
}
