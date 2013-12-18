using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class MeshObject3D : Object3D
	{
		swig.CoreMeshObject3D coreObject = null;

		public MeshObject3D()
		{
			coreObject = Engine.ObjectSystemFactory.CreateMeshObject3D();
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
		/// 描画に使用するメッシュを設定する。
		/// </summary>
		public Mesh Mesh
		{
			get { return null; }
			set { coreObject.SetMesh(value.SwigObject); }
		}

	}
}
