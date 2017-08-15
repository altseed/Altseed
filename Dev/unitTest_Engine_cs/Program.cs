using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs
{
	class TestResult
	{
		public TestResult()
		{
			Exceptions = new Dictionary<GraphicsDeviceType, Exception>();
		}

		public string Title { get; set; }
		public Dictionary<GraphicsDeviceType, Exception> Exceptions { get; private set; }
	}

	class Program
	{
		//[STAThread]
		static void Main(string[] args)
		{
			//TestOnTheDevice(typeof (Graphics._2D.PlayEffectImmediately), GraphicsDeviceType.DirectX11);
			//TestOnTheDevice(typeof(Graphics._2D.PlayEffectImmediately), GraphicsDeviceType.OpenGL);
            TestOnTheDevice(typeof(ObjectSystem2D.Colliding), GraphicsDeviceType.DirectX11);
            TestOnTheDevice(typeof(ObjectSystem2D.Colliding), GraphicsDeviceType.OpenGL);
            Console.ReadKey();
			return;

			TestInNamespace("Graphics._2D");
			Console.ReadKey();
			return;

			TestAll();
			Console.ReadKey();
			return;
		}

		/// <summary>
		/// このプロジェクト内に定義されたテストクラスを収集し、すべて実行する。
		/// </summary>
		private static void TestAll()
		{
			var errors = GetTestTypes()
				.Where(x => !x.Namespace.Contains("BugFix"))
				.Select(TestOnAllDevice)
				.Where(x => x.Exceptions.Any(y => y.Value != null))
				.ToArray();
			ShowErrors(errors);
		}

		private static void TestInNamespace(string namespaceName)
		{
			var errors = GetTestTypes()
				.Where(x => x.Namespace.Contains(namespaceName))
				.Select(TestOnAllDevice)
				.Where(x => x.Exceptions.Any(y => y.Value != null))
				.ToArray();
			ShowErrors(errors);
		}

		/// <summary>
		/// 指定したテストクラスに対して、OpenGL と DirectX 向けのテストを行う。
		/// </summary>
		/// <remarks>個別にテストしたい場合に利用してください。</remarks>
		/// <param name="testType">対象のテストクラス。</param>
		private static TestResult TestOnAllDevice(Type testType)
		{
			TestResult result = new TestResult()
			{
				Title = testType.FullName
			};

			{
				result.Exceptions[GraphicsDeviceType.OpenGL] = TestOnTheDevice(testType, GraphicsDeviceType.OpenGL);
				Task.Delay(20).Wait();
			}

			if(IsDirectXAvailable())
			{
				result.Exceptions[GraphicsDeviceType.DirectX11] = TestOnTheDevice(testType, GraphicsDeviceType.DirectX11);
				Task.Delay(20).Wait();
			}

			return result;
		}

		private static Exception TestOnTheDevice(Type testType, GraphicsDeviceType device)
		{
			try
			{
				var target = (TestFramework)Activator.CreateInstance(testType);
				target.Test(device);
				return null;
			}
			catch(Exception e)
			{
				Console.WriteLine(e.ToString());
				return e;
			}
		}


		private static IEnumerable<Type> GetTestTypes()
		{
			return Assembly.GetAssembly(typeof(Program))
				.GetTypes()
				.Where(_ => !_.IsAbstract)
				.Where(_ => _.IsSubclassOf(typeof(TestFramework)))
				.Where(x => x.GetConstructor(new Type[0]) != null);
		}

		private static void ShowErrors(TestResult[] errors)
		{
			if(errors.Any())
			{
				Console.ForegroundColor = ConsoleColor.Red;
				Console.WriteLine("{0} tests failed.", errors.Length);
				Console.ForegroundColor = ConsoleColor.Gray;
				foreach(var testResult in errors)
				{
					ShowError(testResult);
				}
			}
			else
			{
				Console.ForegroundColor = ConsoleColor.Green;
				Console.WriteLine("All tests succeded.");
				Console.ForegroundColor = ConsoleColor.Gray;
			}
		}

		private static void ShowError(TestResult testResult)
		{
			if(testResult.Exceptions[GraphicsDeviceType.OpenGL] != null)
			{
				Console.ForegroundColor = ConsoleColor.Red;
				Console.WriteLine("[{0} (OpenGL)]", testResult.Title);
				Console.ForegroundColor = ConsoleColor.Gray;
				Console.WriteLine(testResult.Exceptions[GraphicsDeviceType.OpenGL]);
			}
			if(testResult.Exceptions[GraphicsDeviceType.DirectX11] != null)
			{
				Console.ForegroundColor = ConsoleColor.Red;
				Console.WriteLine("[{0} (DirectX)]", testResult.Title);
				Console.ForegroundColor = ConsoleColor.Gray;
				Console.WriteLine(testResult.Exceptions[GraphicsDeviceType.DirectX11]);
			}
		}


		/// <summary>DirectXが有効なシステムか。
		/// </summary>
		/// <returns>DirectXが有効なシステムなら<c>true</c>、そうでなければ<c>false</c>を返す。</returns>
		private static bool IsDirectXAvailable()
		{
			var os = System.Environment.OSVersion;
			var isUnix = os.Platform == PlatformID.Unix || os.Platform == PlatformID.MacOSX;
			return !isUnix;
		}
	}

	public abstract class TestFramework
	{
		public abstract void Test(asd.GraphicsDeviceType graphicsType);
	}
}
