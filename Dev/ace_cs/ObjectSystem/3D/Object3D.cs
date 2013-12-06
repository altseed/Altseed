using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public abstract class Object3D : IDestroy
	{
		internal swig.CoreObject3D commonObject = null;

		Layer3D owner = null;

		public Object3D()
		{
		}

		#region GC対応
		~Object3D()
		{
			Destroy();
		}

		public bool IsDestroyed
		{
			get { return commonObject == null; }
		}

		public virtual void Destroy()
		{
			lock (this)
			{
				if (commonObject == null) return;
				GC.Collector.AddObject(commonObject);
				commonObject = null;
			}
			System.GC.SuppressFinalize(this);
		}
		#endregion


		internal void Start()
		{
			OnStart();
		}

		internal abstract swig.CoreObject3D CoreObject { get { return commonObject; } }
	
		protected abstract void OnStart();
		protected abstract void OnUpdate();
		protected abstract void OnDrawAdditionally();

		public Layer3D Layer
		{
			get { return owner; }
			internal set { owner = value; }
		}

		public Vector3DF GetPosition()
		{
			return commonObject.GetPosition();
		}
	}
}
