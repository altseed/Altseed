using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs
{
	static class Assert
	{
		private static readonly string baseMessage = "アサーションに失敗しました。";

		public static void AreEqual<T>( T expected, T actual )
		{
			if( expected == null && actual == null )
			{
				return;
			}
			if( ( expected == null && actual != null ) || !expected.Equals( actual ) )
			{
				var message = string.Format( "{0} が必要ですが、{1} が指定されました。", expected, actual );
				throw new TestFailedException( baseMessage + message );
			}
		}

		public static void AreNotEqual<T>( T notExpected, T actual )
		{
			if( notExpected == null && actual != null )
			{
				return;
			}
			if( ( notExpected == null && actual == null ) || notExpected.Equals( actual ) )
			{
				var message = string.Format( "値は {0} でない必要があります。", actual );
				throw new TestFailedException( baseMessage + message );
			}
		}

		public static void IsTrue(bool condition)
		{
			if(!condition)
			{
				throw new TestFailedException("式の値は true である必要があります。");
			}
		}

		public static void Throws<TException>(Action action) where TException : Exception
		{
			try
			{
				action();
			}
			catch(TException)
			{
				return;
			}
			throw new TestFailedException(typeof(TException).Name + "の例外が投げられることが期待されましたが、投げられませんでした。");
		}
	}
}
