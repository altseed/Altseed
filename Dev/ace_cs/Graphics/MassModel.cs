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

		/// <summary>
		/// ループするかを取得する。
		/// </summary>
		/// <param name="name">アニメーション名</param>
		/// <returns>ループするか?</returns>
		public bool GetIsLoopingMode(string name)
		{
			return SwigObject.GetIsLoopingMode(name);
		}

		/// <summary>
		/// ループするかを設定する。
		/// </summary>
		/// <param name="name">アニメーション名</param>
		/// <param name="isLoopingMode">ループするか?</param>
		/// <returns></returns>
		public void SetIsLoopingMode(string name, bool isLoopingMode)
		{
			SwigObject.SetIsLoopingMode(name, isLoopingMode);
		}

		/// <summary>
		/// 材質を設定する。
		/// </summary>
		/// <param name="material">材質</param>
		public void SetMaterial(int materialIndex, Material3D material)
		{
			SwigObject.SetMaterial(IG.GetMaterial3D(material));
		}
	}
}
