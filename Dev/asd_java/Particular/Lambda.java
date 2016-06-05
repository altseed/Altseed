package asd.Particular;

import java.util.Iterator;

public class Lambda
{
	public static <TContent> boolean HasContentHavingSpecificUpdatePriority(
		java.util.TreeMap<Integer, java.util.LinkedList<TContent>> contents,
		int updatePriority)
	{
		Iterator<Integer> it = contents.keySet().iterator();
        while (it.hasNext())
		{
			Integer key = it.next();
			if(key == updatePriority) return true;
        }

		return false;

		//return contents.Any(x => x.Key == updatePriority);
	}

	public static java.lang.Iterable<asd.Object2D> FilterDeadObject(java.util.ArrayList<asd.Object2D> objects)
	{
		return objects.stream().filter(x -> x.IsAlive)::iterator;
	}

	public static void RemoveDead(java.util.ArrayList<asd.Object2D> objects)
	{
		objects.RemoveAll(x -> !x.IsAlive);
	}

	public static void SortByUpdatePriority(java.util.ArrayList<asd.Layer> layers)
	{
		layers.Sort((x, y) -> x.UpdatePriority - y.UpdatePriority);
	}

	public static <TContent> java.lang.Iterable<TContent> ToLinear(java.util.TreeMap<Integer, java.util.LinkedList<TContent>> contents)
	{
		return contents.SelectMany(x -> x.Value);
	}

	public static void SortByDrawingPriority(java.util.ArrayList<asd.Layer> layersToDraw)
	{
		layersToDraw.Sort((x, y) -> x.DrawingPriority - y.DrawingPriority);
	}
}
