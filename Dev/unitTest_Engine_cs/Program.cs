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
			/*
			TestSequencially(new ObjectSystem2D.AddComponentByComponent());
			Console.ReadKey();
			return;
			*/

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
				.Select(_ => Activator.CreateInstance(_) as TestFramework)
				.Where(_ => _ != null)
				.ToList()
				.ForEach(TestSequencially);
		}

		/// <summary>
		/// 指定したテストクラスに対して、OpenGL と DirectX 向けのテストを行う。
		/// </summary>
		/// <remarks>個別にテストしたい場合に利用してください。</remarks>
		/// <param name="target">対象のテストクラス。</param>
		private static void TestSequencially(TestFramework target)
		{
			try
			{
				target.Test(ace.GraphicsDeviceType.OpenGL);
			}
			catch (Exception e)
			{
				Console.WriteLine(e.ToString());
			}

			Task.Delay(50).Wait();

			try
			{
				target.Test(ace.GraphicsDeviceType.DirectX11);
			}
			catch (Exception e)
			{
				Console.WriteLine(e.ToString());
			}

			Task.Delay(50).Wait();
		}
	}

	public abstract class TestFramework
	{
		public abstract void Test(ace.GraphicsDeviceType graphicsType);
	}
}
