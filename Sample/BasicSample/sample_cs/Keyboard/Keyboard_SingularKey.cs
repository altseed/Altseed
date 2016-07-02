/// <summary>
/// キーボードの一つのキーの入力状態を取得するサンプル。
/// </summary>
public class Keyboard_SingularKey : ISample
{
    public string Description
    {
        get { return "キーボードのキー入力状態を詳しく調べ、状態を表示するサンプルです。"; }
    }
	public string Title
	{
		get { return "キーボードの入力状態"; }
	}

    public void Run()
    {   
		// Altseedを初期化する。
        asd.Engine.Initialize("Keyboard_SingularKey", 640, 480, new asd.EngineOption());

        var font = asd.Engine.Graphics.CreateDynamicFont("", 40, new asd.Color(255, 255, 255, 255), 1, new asd.Color(0, 0, 0, 255));

        // キーの入力状態を表示する文字描画オブジェクトを設定して、エンジンに追加する。
        var keyStateText = new asd.TextObject2D();
        keyStateText.Position = new asd.Vector2DF(10, 10);
        keyStateText.Font = font;
        asd.Engine.AddObject2D(keyStateText);

        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            // キーボードのZキーの入力状態を取得する。

            var zstate = asd.Engine.Keyboard.GetKeyState(asd.Keys.Z);

            if(zstate == asd.KeyState.Free) // 前フレームと本フレームで非押下
            {
                keyStateText.Text = "Zキーを離しています。";
            }
            else if(zstate == asd.KeyState.Hold) // 前フレームと本フレームで押下
            {
                keyStateText.Text = "Zキーを押しています。";
            }
            else if(zstate == asd.KeyState.Release) // 前フレームで押下、本フレームで非押下
            {
                keyStateText.Text = "Zキーを離しました!";
            }
            else if(zstate == asd.KeyState.Push) // 前フレームで非押下、本フレームで押下
            {
                keyStateText.Text = "Zキーを押しました!";
            }

            // Altseedを更新する。
            asd.Engine.Update();

            Recorder.CaptureScreen("Keyboard_SingularKey", 30, 60, 0.5f, 0.5f);
			Recorder.TakeScreenShot("Keyboard_SingularKey", 30);
        }

        //Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}
