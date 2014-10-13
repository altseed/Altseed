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
	public class MassModel : IDestroy
	{
		internal swig.MassModel SwigObject { get; set; }

		internal MassModel(swig.MassModel swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.MassModels.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			SwigObject = swig;
		}

		~MassModel()
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
