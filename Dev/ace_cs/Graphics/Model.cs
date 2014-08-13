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
