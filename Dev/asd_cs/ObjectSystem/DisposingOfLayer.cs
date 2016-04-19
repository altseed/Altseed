using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	internal class DisposingOfLayer
	{
		public bool DisposeNative { get; private set; }

		public DisposingOfLayer(bool disposeNative)
		{
			DisposeNative = disposeNative;
		}
	}
}
