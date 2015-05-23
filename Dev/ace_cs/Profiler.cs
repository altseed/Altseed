using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public partial class Profiler
	{
		internal Profiler( swig.Profiler profiler )
		{
			this.CoreInstance = profiler;
		}
	}
}
