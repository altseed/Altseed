
class Window_External : ISample
{
	public void Run()
	{
		// 外部のウィンドウを作成する。
		bool closed = false;
		System.Windows.Forms.Form form = new System.Windows.Forms.Form();
		form.FormClosed += (object sender, System.Windows.Forms.FormClosedEventArgs e) =>
		{
			closed = true;
		};
		form.Show();

		// 外部のウィンドウを利用してAltseedを初期化する。
		asd.Engine.InitializeByExternalWindow(form.Handle, System.IntPtr.Zero, form.Size.Width, form.Size.Height, new asd.EngineOption());

		while (asd.Engine.DoEvents())
		{
			// 外部のウィンドウの処理を進める。
			System.Windows.Forms.Application.DoEvents();
			// 外部のウィンドウが閉じられたらAltseed用のゲームループも抜ける。
			if (closed)
			{
				break;
			}

			asd.Engine.Update();
		}

		asd.Engine.Terminate();
	}

	public string Description
	{
		get { return "外部のウィンドウにAltseedの機能で描画を行うサンプルです。"; }
	}

	public string Title
	{
		get { return "外部のウィンドウ"; }
	}

	public string ClassName
	{
		get { return "Window_External"; }
	}
}
