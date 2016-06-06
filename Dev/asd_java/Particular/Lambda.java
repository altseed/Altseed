package asd.Particular;

import java.util.Iterator;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

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
		return objects.stream().filter(x -> x.getIsAlive())::iterator;
	}

	public static void RemoveDead(java.util.ArrayList<asd.Object2D> objects)
	{
		java.util.ArrayList<asd.Object2D> removed = new java.util.ArrayList<asd.Object2D>();
		for(asd.Object2D o : objects)
		{
			if(!o.getIsAlive())
			{
				removed.add(o);
			}
		}
		objects.removeAll(removed);
	}

	public static void SortByUpdatePriority(java.util.ArrayList<asd.Layer> layers)
	{
		Collections.sort(layers,
			new Comparator<asd.Layer>()
			{
				public int compare( asd.Layer x, asd.Layer y ){ return x.getUpdatePriority() - y.getUpdatePriority(); }
			}
		);

		//layers.Sort((x, y) -> x.UpdatePriority - y.UpdatePriority);
	}

	public static <TContent> java.lang.Iterable<TContent> ToLinear(java.util.TreeMap<Integer, java.util.LinkedList<TContent>> contents)
	{
		return contents.values().stream().flatMap(v->v.stream())::iterator;
		//return contents.SelectMany(x -> x.Value);
	}

	public static void SortByDrawingPriority(java.util.ArrayList<asd.Layer> layersToDraw)
	{
		Collections.sort(layersToDraw,
			new Comparator<asd.Layer>()
			{
				public int compare( asd.Layer x, asd.Layer y ){ return x.getDrawingPriority() - y.getDrawingPriority(); }
			}
		);

		//layersToDraw.Sort((x, y) -> x.DrawingPriority - y.DrawingPriority);
	}
}
