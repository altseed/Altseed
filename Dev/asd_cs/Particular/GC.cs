using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd.Particular
{
	class GC
	{
		public static void SuppressFinalize(object obj)
		{
			System.GC.SuppressFinalize(obj);
		}

		public static void CollectAll()
		{
			System.GC.Collect();
			System.GC.WaitForPendingFinalizers();
			System.GC.Collect();
		}
	}
}
