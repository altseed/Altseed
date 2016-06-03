package asd.Particular;

public class GC
{
	public static void SuppressFinalize(Object obj)
	{
		// 対応するメソッドがない
		// ファイナライザが必ず呼ばれる
	}

	public static void CollectAll()
	{
		java.lang.System.gc();
	}
}
