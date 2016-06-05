package asd.Particular;

public class Dictionary
{
	public static <TKey,TValue> TValue Get(java.lang.Dictionary<TKey,TValue> d, TKey key)
	{
		return d.get(key);
	}

	public static <TKey,TValue> void Set(java.lang.Dictionary<TKey,TValue> d, TKey key, TValue value)
	{
		return d.put(key, value);
	}
}
