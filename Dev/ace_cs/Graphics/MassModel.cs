using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 大量描画が可能な3Dモデルクラス
	/// </summary>
	public partial class MassModel : IDestroy
	{
		internal MassModel(swig.MassModel swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.MassModels.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			CoreInstance = swig;
		}

		~MassModel()
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
		public void SetMaterial(Material3D material)
		{
			CoreInstance.SetMaterial(IG.GetMaterial3D(material));
		}
	}
}
