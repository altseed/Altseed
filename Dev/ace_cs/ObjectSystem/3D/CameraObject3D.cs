using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class CameraObject3D : Object3D
	{
		swig.CoreCameraObject3D coreObject = null;

		protected override void OnStart() { }
		protected override void OnUpdate() { }
		protected override void OnDrawAdditionally() { }

		Vector2DI WindowSize
		{
			get { return coreObject.GetWindowSize(); }
			set { coreObject.SetWindowSize(ref value); }
		}

		Vector3DF Focus
		{
			get { return coreObject.GetFocus(); }
			set { coreObject.SetFocus(ref value); }
		}

		float FieldOfView
		{
			get { return coreObject.GetFieldOfView(); }
			set { coreObject.SetFieldOfView(value); }
		}

		float ZFar
		{
			get { return coreObject.GetZFar(); }
			set { coreObject.SetZFar(value); }
		}

		float ZNear
		{
			get { return coreObject.GetZNear(); }
			set { coreObject.SetZNear(value); }
		}
	}
}
