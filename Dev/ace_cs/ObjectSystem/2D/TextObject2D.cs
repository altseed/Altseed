using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class TextObject2D : Object2D, IDestroy
	{

		internal override swig.CoreObject2D CoreObject
		{
			get { return coreTextObject; }
		}
		private swig.CoreTextObject2D coreTextObject { get; set; }

		public TextObject2D()
		{
			coreTextObject = Engine.ObjectSystemFactory.CreateTextObject2D();

			var p = coreTextObject.GetPtr();
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
		~TextObject2D()
		{
			Destroy();
		}

		public override bool IsDestroyed
		{
			get { return coreTextObject == null; }
		}

		public override void Destroy()
		{
			lock( this )
			{
				if( coreTextObject == null ) return;
				GC.Collector.AddObject( coreTextObject );
				coreTextObject = null;
			}
			System.GC.SuppressFinalize( this );
		}
#endregion
	}
}
