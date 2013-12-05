using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class DirectionalLightObject3D : Object3D
	{
		swig.CoreDirectionalLightObject3D coreObject = null;

		protected override void OnStart() { }
		protected override void OnUpdate() { }
		protected override void OnDrawAdditionally() { }

		public Color Color 
		{
			get { return coreObject.GetColor(); }
			set { coreObject.SetColor(ref value); }
		}
	}
}
