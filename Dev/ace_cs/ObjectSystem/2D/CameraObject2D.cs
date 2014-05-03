using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class CameraObject2D : Object2D, IDestroy
	{
		public RectI Src
		{
			get { return coreCameraObject.GetSrc(); }
			set { coreCameraObject.SetSrc( value ); }
		}
		public RectI Dst
		{
			get { return coreCameraObject.GetDst(); }
			set { coreCameraObject.SetDst( value ); }
		}

		internal override swig.CoreObject2D CoreObject
		{
			get { return coreCameraObject; }
		}
		private swig.CoreCameraObject2D coreCameraObject { get; set; }

		public CameraObject2D()
		{
			coreCameraObject = Engine.ObjectSystemFactory.CreateCameraObject2D();

			var p = coreCameraObject.GetPtr();
			if( GC.Object2Ds.GetObject( p ) != null )
			{
				throw new Exception();
			}

			GC.Object2Ds.AddObject( p, this );
		}

		protected override void OnStart()
		{
		}

		protected override void OnUpdate()
		{
		}

		protected override void OnDrawAdditionally()
		{
		}

#region GC対策
		~CameraObject2D()
		{
			Destroy();
		}

		public override bool IsDestroyed
		{
			get { return coreCameraObject == null; }
		}

		public override void Destroy()
		{
			lock( this )
			{
				if( coreCameraObject == null ) return;
				GC.Collector.AddObject( coreCameraObject );
				coreCameraObject = null;
			}
			System.GC.SuppressFinalize( this );
		}
#endregion
	}
}
