package asd.Particular;

public class WeakReference
{
	public static <T> T Get(java.lang.ref.WeakReference<T> w)
	{
		return w.get();
	}
}
