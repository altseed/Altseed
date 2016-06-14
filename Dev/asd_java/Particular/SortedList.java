package asd.Particular;

public class SortedList
{
	public static <TKey,TValue> TValue Get(java.util.TreeMap<TKey,TValue> d, TKey key)
	{
		return d.get(key);
	}

	public static <TKey,TValue> void Set(java.util.TreeMap<TKey,TValue> d, TKey key, TValue value)
	{
		d.put(key, value);
	}
}
