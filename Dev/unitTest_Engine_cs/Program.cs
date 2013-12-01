using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs
{
	class Program
	{
		static void Main( string[] args )
		{
			TestAll();
		}

		private static void TestAll()
		{
			Assembly.GetAssembly( typeof( Program ) )
				.GetTypes()
				.Where( _ => _ != typeof( EngineTest ) )
				.Where( _ => _.IsSubclassOf( typeof( TestFramework ) ) )
				.Select( _ => Activator.CreateInstance( _ ) as TestFramework )
				.Where( _ => _ != null )
				.ToList()
				.ForEach( TestSequencially );
		}

		private static void TestSequencially( TestFramework target )
		{
			try
			{
				target.Test( ace.GraphicsType.OpenGL );
			}
			catch( Exception e )
			{
				Console.WriteLine( e.ToString() );
			}

			Task.Delay( 30 ).Wait();

			try
			{
				target.Test( ace.GraphicsType.DirectX11 );
			}
			catch( Exception e )
			{
				Console.WriteLine( e.ToString() );
			}

			Task.Delay( 30 ).Wait();
		}
	}

	public abstract class TestFramework
	{
		public abstract void Test( ace.GraphicsType graphicsType );
	}
}
