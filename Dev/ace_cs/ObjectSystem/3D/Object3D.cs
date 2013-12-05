using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public abstract class Object3D
	{
		internal swig.CoreObject3D commonObject = null;

		Layer3D owner = null;

		internal void Start()
		{
			OnStart();
		}

		internal void SetLayer(Layer3D layer)
		{
			owner = layer;
		}

		internal swig.CoreObject3D GetCoreObject()
		{
			return commonObject;
		}
	
		protected abstract void OnStart();
		protected abstract void OnUpdate();
		protected abstract void OnDrawAdditionally();

		public Layer3D GetLayer()
		{
			return owner;
		}

		public Vector3DF GetPosition()
		{
			return commonObject.GetPosition();
		}
	}
}
