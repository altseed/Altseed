using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd.Particular
{
	class Define
	{
#if DEBUG
		public const string DLL = "Altseed_core.Debug";
#else
		public const string DLL = "Altseed_core";
#endif
	}
}
