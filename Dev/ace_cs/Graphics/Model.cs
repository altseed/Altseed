using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 3Dモデルクラス
	/// </summary>
	public class Model : IDestroy
	{
		internal swig.Model SwigObject { get; set; }

		internal Model(swig.Model swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.Models.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			SwigObject = swig;
		}

		~Model()
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
		/// モデルが持つアニメーションクリップを取得する。
		/// </summary>
		/// <param name="index">アニメーションクリップのインデックス</param>
		/// <returns>アニメーションクリップ</returns>
		public AnimationClip GetAnimationClip(int index)
		{
			return GC.GenerateAnimationClip(SwigObject.GetAnimationClip(index), GC.GenerationType.Get);
		}

		/// <summary>
		/// モデルが持つアニメーションクリップの名称を取得する。
		/// </summary>
		/// <param name="index">アニメーションクリップのインデックス</param>
		/// <returns>アニメーションクリップの名称</returns>
		public string GetAnimationClipName(int index)
		{
			return SwigObject.GetAnimationClipName(index);
		}

		/// <summary>
		/// モデルが持つアニメーションクリップの個数を取得する。
		/// </summary>
		public int AnimationClipCount
		{
			get
			{
				return SwigObject.GetAnimationClipCount();
			}
		}

		/// <summary>
		/// メッシュを取得する。
		/// </summary>
		/// <param name="index">メッシュのインデックス</param>
		/// <returns></returns>
		public Mesh GetMesh(int index)
		{
			return GC.GenerateMesh(swig.Accessor.Model_GetMesh(SwigObject, index), GC.GenerationType.Get);
		}

		/// <summary>
		/// モデルが持つメッシュの個数を取得する。
		/// </summary>
		public int MeshCount
		{
			get { return SwigObject.GetMeshCount(); }
		}
	}
}
