using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class Profiler// : IDestroy
	{
		internal Profiler( swig.Profiler profiler )
		{
			// CoreからのGet経由のみなら必要ない
			this.profiler = profiler;
			//GC.Profilers.AddObject(profiler.GetPtr(), this);
		}

		//~Profiler()
		//{
		//	Destroy();
		//}
		//
		//public bool IsDestroyed
		//{
		//	get { return profiler == null; }
		//}
		//
		//public void Destroy()
		//{
		//	lock( this )
		//	{
		//		if( IsDestroyed ) return;
		//		GC.Collector.AddObject( profiler );
		//		profiler = null;
		//	}
		//	System.GC.SuppressFinalize( this );
		//}

		public void Start( int id )
		{
			profiler.Start( id );
		}

		public void End( int id )
		{
			profiler.End( id );
		}

		private swig.Profiler profiler;
	}
}
