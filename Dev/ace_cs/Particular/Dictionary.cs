using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace.Particular
{
	class Dictionary
	{
		public static void Set<TKey, TValue>(Dictionary<TKey,TValue> d, TKey key, TValue value)
		{
			d[key] = value;
		}

		public static TValue Get<TKey, TValue>(Dictionary<TKey,TValue> d, TKey key)
		{
			return d[key];
		}
	}
}
