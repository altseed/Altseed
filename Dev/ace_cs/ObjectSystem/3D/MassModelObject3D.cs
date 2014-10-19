using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 大量にメッシュを表示する3Dオブジェクトの機能を提供するクラス
	/// </summary>
	public class MassModelObject3D : Object3D
	{
		swig.CoreMassModelObject3D coreObject = null;

		public MassModelObject3D()
		{
			coreObject = Engine.ObjectSystemFactory.CreateMassModelObject3D();
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

		/// <summary>
		/// 描画に使用するモデルを設定する。
		/// </summary>
		/// <param name="model">モデル</param>
		public void SetModel(MassModel model)
		{
			coreObject.SetModel(model.SwigObject);
		}
	}
}
