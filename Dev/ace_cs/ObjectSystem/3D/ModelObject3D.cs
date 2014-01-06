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
		/// 描画に使用するメッシュを設定する。
		/// </summary>
		public void AddMesh(Mesh mesh)
		{
			coreObject.AddMesh(mesh.SwigObject);
		}

	}
}
