
class EngineMisc_ShowFps : ISample
{
    public string Description
    {
        get { return ""; }
    }
	public string Title
	{
		get { return ""; }
	}
	public string ClassName
	{
		get { return "EngineMisc_ShowFps"; }
	}

    // 現在のFPSを画面に表示するサンプル。
    public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("EngineMisc_ShowFps", 640, 480, new asd.EngineOption());

		// 動的フォントを生成する。
		var font = asd.Engine.Graphics.CreateDynamicFont("", 32, new asd.Color(255, 255, 255, 255), 1, new asd.Color(255, 255, 255, 255));

		// FPSを表示するためのオブジェクトを生成する。
		var obj = new asd.TextObject2D();
		obj.Font = font;

		// オブジェクトをエンジンに追加する。
		asd.Engine.AddObject2D(obj);

		while(asd.Engine.DoEvents())
		{
			asd.Engine.Update();

			// 現在のFPSを取得する。
			float fps = asd.Engine.CurrentFPS;

			// 表示する文字列を生成する。
			var str = "FPS : " + fps; 

			// 文字列をオブジェクトに設定する。
			obj.Text = str;

			Recorder.TakeScreenShot("Fps_ShowFps", 30);
		}

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}

