using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class EngineOption
	{
		/// <summary>
		/// フルスクリーンで起動するか?
		/// </summary>
		public bool IsFullScreen;

		/// <summary>
		/// 描画に使用するデバイス
		/// </summary>
		public GraphicsDeviceType GraphicsDevice;

		/// <summary>
		/// マルチスレッドモードを使用するか?
		/// </summary>
		/// <remarks>
		/// 現在、マルチスレッドモードは開発中であり、実行速度は上昇するが、バグが多発する可能性がある。
		/// </remarks>
		public bool IsMultithreadingMode = false;
	};

	public class Engine
	{
		static swig.Core_Imp core = null;

		/// <summary>
		/// 現在描画対象となっているシーンを表す ace.Scene クラスのインスタンスを取得します。
		/// </summary>
		public static Scene CurrentScene { get; private set; }
		public static Log Logger { get; private set; }
		public static Profiler Profiler { get; private set; }
		public static Keyboard Keyboard { get; private set; }
		public static Mouse Mouse { get; private set; }
		public static JoystickContainer JoystickContainer { get; private set; }
		public static Sound Sound { get; private set; }
		public static Graphics Graphics { get; private set; }
		public static AnimationSystem AnimationSystem { get; private set; }

		internal static ObjectSystemFactory ObjectSystemFactory { get; private set; }
		private static Scene NextScene { get; set; }


		/// <summary>
		/// 初期化を行う。
		/// </summary>
		/// <param name="title">タイトル</param>
		/// <param name="width">横幅</param>
		/// <param name="height">縦幅</param>
		/// <param name="option">オプション</param>
		/// <returns>成否</returns>
		public static bool Initialize(string title, int width, int height, EngineOption option)
		{
			if (core != null) return false;

			//if (!CheckDLL()) return false;

			try
			{
				core = swig.Core_Imp.CreateCore();
			}
			catch (System.TypeInitializationException)
			{
				// DLLが見つからないケース
				return false;
			}

			var graphicsType = option.GraphicsDevice;
			if (graphicsType == GraphicsDeviceType.Default)
			{
				graphicsType = GraphicsDeviceType.DirectX11;
			}

			var result = core.Initialize(title, width, height, option.IsFullScreen, graphicsType == GraphicsDeviceType.OpenGL, option.IsMultithreadingMode);

			if (result)
			{
				GC.Initialize();
				SetupMembers();
				return true;
			}
			else
			{
				core.Release();
				core = null;
				return false;
			}
		}

		/// <summary>
		/// 外部ウインドウにゲーム画面を表示する初期化を行う。
		/// </summary>
		/// <param name="handle1">ハンドル1</param>
		/// <param name="handle2">ハンドル2</param>
		/// <param name="width">横幅</param>
		/// <param name="height">縦幅</param>
		/// <param name="option">オプション</param>
		/// <returns>成否</returns>
		public static bool InitializeByExternalWindow(IntPtr handle1, IntPtr handle2, int width, int height, EngineOption option)
		{
			if (core != null) return false;

			if (!CheckDLL()) return false;

			try
			{
				core = swig.Core_Imp.CreateCore();
			}
			catch (System.TypeInitializationException)
			{
				// DLLが見つからないケース
				return false;
			}

			var graphicsType = option.GraphicsDevice;
			if (graphicsType == GraphicsDeviceType.Default)
			{
				graphicsType = GraphicsDeviceType.DirectX11;
			}

			var result = core.InitializeByExternalWindow(handle1, handle2, width, height, graphicsType == GraphicsDeviceType.OpenGL, option.IsMultithreadingMode);

			if (result)
			{
				GC.Initialize();
				SetupMembers();
				return true;
			}
			else
			{
				core.Release();
				core = null;
				return false;
			}
		}

		/// <summary>
		/// イベントを実行し、進行可否を判断する。
		/// </summary>
		/// <returns>進行可能か?</returns>
		public static bool DoEvents()
		{
			if (core == null) return false;

			GC.Update();

			bool mes = core.DoEvents();

			if (Mouse != null)
			{
				Mouse.RefreshAllState();
			}

			if( NextScene != null )
			{
				CurrentScene = NextScene;
				core.ChangeScene( NextScene.CoreScene );
				NextScene = null;
			}

			return mes;
		}

		/// <summary>
		/// 更新処理を行う。
		/// </summary>
		public static void Update()
		{
			if (core == null) return;

			core.BeginDrawing();

			if (CurrentScene != null)
			{
				CurrentScene.Update();
			}

			if (CurrentScene != null)
			{
				CurrentScene.Draw();
			}

			if (CurrentScene != null)
			{
				core.DrawSceneToWindow(CurrentScene.CoreScene);
			}

			core.Draw();

			core.EndDrawing();

		}

		/// <summary>
		/// 終了処理を行う。
		/// </summary>
		public static void Terminate()
		{
			if (core == null) return;

			CurrentScene = null;

			GC.Terminate();

			core.Terminate();
			core.Release();
			core = null;

			Mouse = null;

			var refCount = swig.ace_core_cs.GetGlobalReferenceCount__();

			if (refCount > 0)
			{
				//Console.WriteLine("未開放のインスタンスが{0}個存在します。", refCount);
				throw new Exception(string.Format("未開放のインスタンスが{0}個存在します。", refCount));
			}
		}

		/// <summary>
		/// 描画の対象となるシーンを変更します。
		/// </summary>
		/// <param name="scene">新しく描画の対象となるシーン。</param>
		public static void ChangeScene(Scene scene)
		{
			NextScene = scene;
		}

		/// <summary>
		/// ウインドウを閉じる。
		/// </summary>
		public static void Close()
		{
			core.Close();
		}

		/// <summary>
		/// スクリーンショットをpngとして保存する。
		/// </summary>
		/// <param name="path">出力先</param>
		public static void TakeScreenshot(string path)
		{
			core.TakeScreenshot(path);
		}

		/// <summary>
		/// 現在のFPSを取得する。
		/// </summary>
		public static float CurrentFPS
		{
			get
			{
				return core.GetCurrentFPS();
			}
		}

		/// <summary>
		/// 目標FPSを取得、または設定する。
		/// </summary>
		public static int TargetFPS
		{
			get
			{
				return core.GetTargetFPS();
			}
			set
			{
				core.SetTargetFPS(value);
			}
		}

		/// <summary>
		/// ウインドウズの場合、ウインドウハンドルを取得する。
		/// </summary>
		public static IntPtr WindowHandle
		{
			get
			{
				return core.GetWindowHandle();
			}
		}

		private static void SetupMembers()
		{
			CurrentScene = null;
			Logger = new Log(core.GetLogger());
			Keyboard = new Keyboard(core.GetKeyboard());

			if (core.GetMouse() != null)
			{
				Mouse = new Mouse(core.GetMouse());
			}

			if (core.GetJoystickContainer() != null)
			{
				JoystickContainer = new JoystickContainer(core.GetJoystickContainer());
			}

			Sound = new Sound(core.GetSound());
			Graphics = new Graphics(core.GetGraphics_Imp());
			ObjectSystemFactory = new ace.ObjectSystemFactory(core.GetObjectSystemFactory());
			Profiler = new Profiler(core.GetProfiler());
			AnimationSystem = new AnimationSystem(core.GetAnimationSyatem());
		}

		[System.Runtime.InteropServices.DllImport("kernel32")]
		private extern static int LoadLibrary(string path);

		[System.Runtime.InteropServices.DllImport("kernel32")]
		private extern static bool FreeLibrary(int path);

		static bool CheckDLL()
		{
			return true;

			if (!HasDLL("D3DCOMPILER_47.dll"))
			{
				System.Windows.Forms.MessageBox.Show("最新のDirectXEndUserRuntime？をインストールしてください。");
				return false;
			}
			return true;
		}

		static bool HasDLL(string path)
		{
			var dll = LoadLibrary(path);
			if(dll != 0)
			{
				FreeLibrary(dll);
				return true;
			}
			return false;
		}
	}
}
