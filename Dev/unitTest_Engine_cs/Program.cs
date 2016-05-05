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
		[STAThread]
		static void Main(string[] args)
		{
			//*
			TestSequencially(typeof(ObjectSystem2D.ReuseLayer));
			Console.ReadKey();
			return;
			//*/

			TestAll();

			Console.ReadKey();
		}

		/// <summary>
		/// このプロジェクト内に定義されたテストクラスを収集し、すべて実行する。
		/// </summary>
		private static void TestAll()
		{
			Assembly.GetAssembly(typeof(Program))
				.GetTypes()
				.Where(_ => !_.IsAbstract)
				.Where(_ => _.IsSubclassOf(typeof(TestFramework)))
				.Where(x => x.GetConstructor(new Type[0]) != null)
				.ToList()
				.ForEach(TestSequencially);
		}

		/// <summary>DirectXが有効なシステムか。
		/// </summary>
		/// <returns>DirectXが有効なシステムなら<c>true</c>、そうでなければ<c>false</c>を返す。</returns>
		private static bool IsDirectXAvailable(){
			var os = System.Environment.OSVersion;
			var isUnix = os.Platform == PlatformID.Unix || os.Platform == PlatformID.MacOSX;
			return !isUnix;
		}

		/// <summary>
		/// 指定したテストクラスに対して、OpenGL と DirectX 向けのテストを行う。
		/// </summary>
		/// <remarks>個別にテストしたい場合に利用してください。</remarks>
		/// <param name="testType">対象のテストクラス。</param>
		private static void TestSequencially(Type testType)
		{
			try
			{
                var target = Activator.CreateInstance(testType) as TestFramework;
                if(target != null)
                {
                    target.Test(asd.GraphicsDeviceType.OpenGL);
                }
			}
			catch (Exception e)
			{
				Console.WriteLine(e.ToString());
			}

			Task.Delay(50).Wait();

			if ( IsDirectXAvailable() ) {
				try
                {
                    var target = Activator.CreateInstance(testType) as TestFramework;
                    if(target != null)
                    {
                        target.Test(asd.GraphicsDeviceType.DirectX11);
                    }
				}
				catch (Exception e)
				{
					Console.WriteLine(e.ToString());
				}

				Task.Delay(50).Wait();
			}
		}
	}

	public abstract class TestFramework
	{
		public abstract void Test(asd.GraphicsDeviceType graphicsType);
	}
}
