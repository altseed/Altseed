using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public struct EngineOption
	{
		/// <summary>
		/// フルスクリーンで起動するか?
		/// </summary>
		public bool IsFullScreen;

		/// <summary>
		/// 描画に使用するデバイス
		/// </summary>
		public GraphicsType GraphicsType;
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
		public static Graphics Graphics { get; private set; }
		internal static ObjectSystemFactory ObjectSystemFactory { get; private set; }


		/// <summary>
		/// 初期化を行う。
		/// </summary>
		/// <param name="title">タイトル</param>
		/// <param name="width">横幅</param>
		/// <param name="height">縦幅</param>
		/// <param name="option">オプション</param>
		/// <returns>成否</returns>
		public static bool Initialize( string title, int width, int height, EngineOption option )
		{
			if( core != null ) return false;

			try
			{
				core = swig.Core_Imp.CreateCore();
			}
			catch( System.TypeInitializationException )
			{
				// DLLが見つからないケース
				return false;
			}

			var result = core.Initialize( title, width, height, option.IsFullScreen, option.GraphicsType == GraphicsType.OpenGL );

			GC.Initialize();

			SetupMembers();

			if( result )
			{
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
			if( core != null ) return false;

			try
			{
				core = swig.Core_Imp.CreateCore();
			}
			catch( System.TypeInitializationException )
			{
				// DLLが見つからないケース
				return false;
			}

			var result = core.InitializeByExternalWindow( handle1, handle2, width, height );

			GC.Initialize();

			SetupMembers();

			if( result )
			{
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
			if( core == null ) return false;

			GC.Update();

			bool mes = core.DoEvents();
			Mouse.RefreshAllState();
			return mes;
		}

		/// <summary>
		/// 更新処理を行う。
		/// </summary>
		public static void Update()
		{
			if( core == null ) return;

			//core.Update();
			if( CurrentScene != null )
			{
				CurrentScene.Update();
			}

			core.BeginDrawing();

			if (CurrentScene != null)
			{
				CurrentScene.BeginDrawing();
			}

			core.Draw();
			
			if( CurrentScene != null )
			{
				CurrentScene.DrawAdditionally();
			}
			
			if (CurrentScene != null)
			{
				CurrentScene.EndDrawing();
			}

			core.EndDrawing();

		}

		/// <summary>
		/// 終了処理を行う。
		/// </summary>
		public static void Terminate()
		{
			if( core == null ) return;

			GC.Terminate();

			core.Terminate();
			core.Release();
			core = null;

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
		public static void ChangeScene( Scene scene )
		{
			CurrentScene = scene;
			core.ChangeScene( scene.CoreScene );
		}

		/// <summary>
		/// スクリーンショットをpngとして保存する。
		/// </summary>
		/// <param name="path">出力先</param>
		public static void TakeScreenshot(string path)
		{
			core.TakeScreenshot(path);
		}

		private static void SetupMembers()
		{
			Logger = new Log( core.GetLogger() );
			Keyboard = new Keyboard( core.GetKeyboard() );
			Mouse = new Mouse( core.GetMouse() );
			JoystickContainer = new JoystickContainer( core.GetJoystickContainer() );
			Graphics = new Graphics( core.GetGraphics_Imp() );
			ObjectSystemFactory = new ace.ObjectSystemFactory( core.GetObjectSystemFactory() );
			Profiler = new Profiler( core.GetProfiler() );
		}
	}
}
