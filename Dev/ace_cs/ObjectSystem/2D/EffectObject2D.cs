using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class EffectObject2D : Object2D, IDestroy
	{
		
		internal override swig.CoreObject2D CoreObject
		{
			get { return coreEffectObject; }
		}
		private swig.CoreEffectObject2D coreEffectObject { get; set; }

		public EffectObject2D()
		{
			coreEffectObject = Engine.ObjectSystemFactory.CreateEffectObject2D();

			var p = coreEffectObject.GetPtr();
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
		~EffectObject2D()
		{
			Destroy();
		}

		public override bool IsDestroyed
		{
			get { return coreEffectObject == null; }
		}

		public override void Destroy()
		{
			lock( this )
			{
				if( coreEffectObject == null ) return;
				GC.Collector.AddObject( coreEffectObject );
				coreEffectObject = null;
			}
			System.GC.SuppressFinalize( this );
		}
#endregion
	}
}
