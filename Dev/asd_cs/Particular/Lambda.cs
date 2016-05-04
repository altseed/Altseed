using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd.Particular
{
	internal static class Lambda
	{
		public static bool HasContentHavingSpecificUpdatePriority<TContent>(
			SortedList<int, LinkedList<TContent>> contents,
			int updatePriority)
		{
			return contents.Any(x => x.Key == updatePriority);
		}

		public static IEnumerable<Object2D> FilterDeadObject(List<Object2D> objects)
		{
			return objects.Where(x => x.IsAlive);
		}

		public static void RemoveDead(List<Object2D> objects)
		{
			objects.RemoveAll(x => !x.IsAlive);
		}

		public static void SortByUpdatePriority(List<Layer> layers)
		{
			layers.Sort((x, y) => x.UpdatePriority - y.UpdatePriority);
		}

		public static IEnumerable<TContent> ToLinear<TContent>(SortedList<int, LinkedList<TContent>> contents)
		{
			return contents.SelectMany(x => x.Value);
		}

		public static void SortByDrawingPriority(List<Layer> layersToDraw)
		{
			layersToDraw.Sort((x, y) => x.DrawingPriority - y.DrawingPriority);
		}
	}
}