package asd.Particular;

public class Lambda
{
	public static bool HasContentHavingSpecificUpdatePriority<TContent>(
		java.util.TreeMap<Int, java.util.LinkedList<TContent>> contents,
		int updatePriority)
	{
		return contents.Any(x => x.Key == updatePriority);
	}

	public static java.lang.Iterable<asd.Object2D> FilterDeadObject(java.util.ArrayList<asd.Object2D> objects)
	{
		return objects.Where(x => x.IsAlive);
	}

	public static void RemoveDead(java.util.ArrayList<asd.Object2D> objects)
	{
		objects.RemoveAll(x => !x.IsAlive);
	}

	public static void SortByUpdatePriority(java.util.ArrayList<asd.Layer> layers)
	{
		layers.Sort((x, y) => x.UpdatePriority - y.UpdatePriority);
	}

	public static java.lang.Iterable<TContent> ToLinear<TContent>(java.util.TreeMap<Int, java.util.LinkedList<TContent>> contents)
	{
		return contents.SelectMany(x => x.Value);
	}

	public static void SortByDrawingPriority(java.util.ArrayList<asd.Layer> layersToDraw)
	{
		layersToDraw.Sort((x, y) => x.DrawingPriority - y.DrawingPriority);
	}
}
