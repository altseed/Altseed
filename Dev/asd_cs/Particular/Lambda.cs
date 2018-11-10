using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd.Particular
{
	internal static class Lambda
	{
		static BucketSort<Object2D> updateObject2DSorter = new BucketSort<Object2D>((_)=> _.UpdatePriority);
		static BucketSort<Object2D> drawObject2DSorter = new BucketSort<Object2D>((_) => _.UpdatePriority);
		static BucketSort<Layer> updateLayerSorter = new BucketSort<Layer>((_) => _.UpdatePriority);
		static BucketSort<Layer> drawLayerSorter = new BucketSort<Layer>((_) => _.UpdatePriority);

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
			updateLayerSorter.Sort(layers);
		}

		public static IEnumerable<TContent> ToLinear<TContent>(SortedList<int, LinkedList<TContent>> contents)
		{
			return contents.SelectMany(x => x.Value);
		}

		public static void SortByDrawingPriority(List<Layer> layersToDraw)
		{
			drawLayerSorter.Sort(layersToDraw);
		}

		class BucketSort<T>
		{
			Func<T, int> getPriority = null;
			SortedDictionary<int, List<T>> tempObjects = new SortedDictionary<int, List<T>>();

			public BucketSort(Func<T, int> getPriorityFunc)
			{
				getPriority = getPriorityFunc;
			}
			public void Sort(List<T> objects)
			{
				foreach(var o in objects)
				{
					var priority = getPriority(o);
					if(!tempObjects.ContainsKey(priority))
					{
						tempObjects.Add(priority, new List<T>());
					}
					tempObjects[priority].Add(o);
				}

				objects.Clear();

				foreach(var to in tempObjects)
				{
					objects.AddRange(to.Value);
					to.Value.Clear();
				}
			}
		}
	}
}