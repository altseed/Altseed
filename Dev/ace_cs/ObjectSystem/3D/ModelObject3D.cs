using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// メッシュを表示する3Dオブジェクトの機能を提供するクラス
	/// </summary>
	public class ModelObject3D : Object3D
	{
		swig.CoreModelObject3D coreObject = null;

		public ModelObject3D()
		{
			coreObject = Engine.ObjectSystemFactory.CreateModelObject3D();
			GC.Object3Ds.AddObject(coreObject.GetPtr(), this);
			commonObject = coreObject;
		}

		public override void Destroy()
		{
			coreObject = null;
			base.Destroy();
		}

		protected override void OnStart() { }
		protected override void OnUpdate() { }
		internal protected override void OnDrawAdditionally() { }

		/// <summary>
		/// 描画に使用するモデルを設定する。
		/// </summary>
		/// <param name="model">モデル</param>
		public void SetModel(Model model)
		{
			coreObject.SetModel(model.SwigObject);
		}

		/// <summary>
		/// メッシュグループを追加する。
		/// </summary>
		public void AddMeshGroup()
		{
			coreObject.AddMeshGroup();
		}

		/// <summary>
		/// メッシュグループの個数を取得する。
		/// </summary>
		/// <returns>個数</returns>
		public int GetMeshGroupCount()
		{
			return coreObject.GetMeshGroupCount();
		}

		/// <summary>
		/// 描画に使用するメッシュを追加する。
		/// </summary>
		/// <param name="meshGroupIndex">メッシュグループのインデックス</param>
		/// <param name="mesh">メッシュ</param>
		public void AddMesh(int meshGroupIndex, Mesh mesh)
		{
			coreObject.AddMesh(meshGroupIndex, mesh.SwigObject);
		}

		/// <summary>
		/// 描画に使用するデフォーマーを設定する。
		/// </summary>
		/// <param name="meshGroupIndex">メッシュグループのインデックス</param>
		/// <param name="deformer">デフォーマー</param>
		public void SetDeformer(int meshGroupIndex, Deformer deformer)
		{
			coreObject.SetDeformer(meshGroupIndex, deformer.SwigObject);
		}
	}
}
