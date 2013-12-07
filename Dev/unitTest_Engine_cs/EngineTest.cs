using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace unitTest_Engine_cs
{
	abstract class EngineTest : TestFramework
	{
		static readonly string PathOfSS = "TestSS_cs";
		protected static readonly string CloudTexturePath = "Data/Texture/Cloud1.png";

		public EngineTest( int timeForExit )
		{
			this.Title = GetType().FullName.Replace( "unitTest_Engine_cs.", "" );
			this.TimeForExit = timeForExit;
		}

		public string Title { get; private set; }
		public int TimeForExit { get; private set; }

		public override void Test( GraphicsType graphicsType )
		{
			Console.WriteLine( "[{0}_{1}]", Title, graphicsType );

			int time = 0;
			var option = new EngineOption
			{
				IsFullScreen = false,
				GraphicsType = graphicsType
			};
			var result = Engine.Initialize( Title, 640, 480, option );

			if( !result )
			{
				throw new Exception( "ACEの初期化に失敗しました。" );
			}

			OnStart();

			while( Engine.DoEvents() )
			{
				OnUpdating();
				Engine.Update();
				OnUpdated();

				if( time == TimeForExit )
				{
					if( !Directory.Exists( PathOfSS ) )
					{
						Directory.CreateDirectory( PathOfSS );
					}
					var path = string.Format( "{0}/{1}_{2}.png", PathOfSS, Title, graphicsType );
					Engine.TakeScreenshot( path );
				}
				else if( time == TimeForExit + 20 )
				{
					break;
				}

				++time;
			}

			OnFinish();

			Engine.Terminate();
		}

		protected virtual void OnFinish()
		{
		}

		protected virtual void OnUpdated()
		{
		}

		protected virtual void OnUpdating()
		{
		}

		protected virtual void OnStart()
		{
		}
	}
}
