package asd.Particular;

public class Dictionary
{
	public static <TKey,TValue> TValue Get(java.util.Map<TKey,TValue> d, TKey key)
	{
		return d.get(key);
	}

	public static <TKey,TValue> void Set(java.util.Map<TKey,TValue> d, TKey key, TValue value)
	{
		d.put(key, value);
	}

	public static <TKey, TValue> java.util.Set<java.util.Map.Entry<TKey, TValue>> EntrySet(java.util.Map<TKey, TValue> d)
	{
		return d.entrySet();
	}
}
