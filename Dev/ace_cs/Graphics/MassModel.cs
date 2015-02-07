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
		/// モデルが持つアニメーションの個数を取得する。
		/// </summary>
		public int AnimationCount
		{
			get { return SwigObject.GetAnimationCount();}
		}

		/// <summary>
		/// モデルが持つアニメーションの名称を取得する。
		/// </summary>
		/// <param name="index">アニメーションのインデックス</param>
		/// <returns>アニメーションの名称</returns>
		public string GetAnimationName(int index)
		{
			return SwigObject.GetAnimationName(index);
		}

		/// <summary>
		/// アニメーションがループするかを取得する。
		/// </summary>
		/// <param name="name">アニメーション名</param>
		/// <returns>ループするか?</returns>
		public bool GetIsAnimationLoopingMode(string name)
		{
			return SwigObject.GetIsAnimationLoopingMode(name);
		}

		/// <summary>
		/// アニメーションがループするかを設定する。
		/// </summary>
		/// <param name="name">アニメーション名</param>
		/// <param name="isLoopingMode">ループするか?</param>
		/// <returns></returns>
		public void SetIsAnimationLoopingMode(string name, bool isLoopingMode)
		{
			SwigObject.SetIsAnimationLoopingMode(name, isLoopingMode);
		}

		/// <summary>
		/// 材質を設定する。
		/// </summary>
		/// <param name="material">材質</param>
		public void SetMaterial(Material3D material)
		{
			SwigObject.SetMaterial(IG.GetMaterial3D(material));
		}
	}
}
