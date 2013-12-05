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

		protected override void OnStart() { }
		protected override void OnUpdate() { }
		protected override void OnDrawAdditionally() { }

		public Mesh Mesh
		{
			get { return null; }
			set { coreObject.SetMesh(value.SwigObject); }
		}

	}
}
