using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd.Particular
{
	class WeakReference
	{
		public static T Get<T>(WeakReference<T> w) where T : class
		{
			T t = default(T);
			w.TryGetTarget(out t);
			return t;
		}
	}
}
