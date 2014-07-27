using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace sample_cs
{
	static class Helper
	{
		public static void ForEach<T>( this IEnumerable<T> source, Action<T, int> action )
		{
			int index = 0;
			foreach( var item in source )
			{
				action( item, index );
				index++;
			}
		}

		public static void DrawLine()
		{
			Console.Write( string.Concat( Enumerable.Repeat( '-', Console.BufferWidth ) ) );
		}
	}

	class SampleGuide
	{
		public SampleGuide( ISample[] samples )
		{
			this.samples = samples;
		}

		public void Show()
		{
			while( true )
			{
				Console.Clear();
				ShowTitles();
				int number = InputSampleNumber();

				if( number == samples.Length )
				{
					break;
				}

				bool run = EnsureToRun( number );
				if( run )
				{
					samples[number].Run();
					Console.WriteLine( "続行するには何かキーを押してください" );
					Console.ReadKey();
				}
			}
		}

		private void ShowTitles()
		{
			Console.WriteLine( "実行するサンプルプログラムの番号を入力してください。" );
			Helper.DrawLine();
			samples.ForEach( ( _, i ) =>
			{
				var type = _.GetType();
				var title = type.Namespace + "." + type.Name;
				Console.WriteLine( "[{0}] {1}", i, title.Replace( "sample_cs.", "" ) );
			} );
			Console.WriteLine( "[{0}] 終了", samples.Length );
			Helper.DrawLine();
		}

		private int InputSampleNumber()
		{
			while( true )
			{
				int result = -1;
				Console.Write( "> " );
				try
				{
					result = int.Parse( Console.ReadLine() );
				}
				catch( FormatException )
				{
					continue;
				}

				if( 0 <= result && result <= samples.Length )
				{
					return result;
				}
			}
		}

		private bool EnsureToRun( int result )
		{
			Helper.DrawLine();
			Console.WriteLine( samples[result].GetType().Name );

			string responce = null;
			while( true )
			{
				Console.Write( "実行しますか？(y/n) > " );
				responce = Console.ReadLine();
				if( responce == "y" )
				{
					return true;
				}
				else if( responce == "n" )
				{
					return false;
				}
			}
		}

		private ISample[] samples;
	}
}
