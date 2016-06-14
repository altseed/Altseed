using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd.Particular
{
	class ChildManagementMode
	{
		public static int ToInt(asd.ChildManagementMode v)
		{
			return (int)v;
		}

		public static int And(asd.ChildManagementMode v1, asd.ChildManagementMode v2)
		{
			return (int)(v1 & v2);
		}
	}
}
