using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	public partial class Profiler
	{
		internal Profiler( swig.Profiler profiler )
		{
			this.CoreInstance = profiler;
		}
	}
}
