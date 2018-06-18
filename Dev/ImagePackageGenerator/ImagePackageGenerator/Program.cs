using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Media;
using System.Windows.Interop;
using System.Diagnostics;

namespace ImagePackageGenerator
{
	class Program
	{
		[STAThread]
		static void Main(string[] args)
		{
			/*
			Application application = new Application();
			application.DispatcherUnhandledException += new System.Windows.Threading.DispatcherUnhandledExceptionEventHandler(application_DispatcherUnhandledException);

			var window = new MainWindow();
			window.Show();

			application.Run();
			*/

			//System.Windows.Forms.Application.Run(new Window());

			var options = new asd.EngineOption();
			options.IsWindowResizable = true;

			asd.Engine.Initialize("ImagePackageGenerator", 960, 540, options);

			asd.Engine.OpenTool();

			// Font
			TryLoadFont(
				new[] {
					"C:\\Windows\\Fonts\\meiryo.ttc",
					"/Library/Fonts/ヒラギノ丸ゴ Pro W4.otf",
					"/Library/Fonts/ヒラギノ丸ゴ ProN W4.ttc",
					"/System/Library/Fonts/ヒラギノ丸ゴ ProN W4.ttc",
				}
				);

			var scene = new ToolScene();
			asd.Engine.ChangeScene(scene);
			while(asd.Engine.DoEvents())
			{
				asd.Engine.Update();
			}

			asd.Engine.Terminate();
		}

		static void TryLoadFont(string[] fonts)
		{
			foreach(var font in fonts)
			{
				if (!System.IO.File.Exists(font)) continue;
				asd.Engine.Tool.AddFontFromFileTTF(font, 14);
				break;
			}
		}

		static void application_DispatcherUnhandledException(object sender, System.Windows.Threading.DispatcherUnhandledExceptionEventArgs e)
		{
			e.Handled = true;
		}
	}
}
