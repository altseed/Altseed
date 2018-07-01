/// <summary>
/// キーボードによる入力を取得するサンプル。
/// </summary>
public class Keyboard_Basic :ISample
{
    public string Description
    {
        get { return "Zキーを押しているかどうかを調べ、結果を表示するサンプルです。"; }
    }
	public string Title
	{
		get { return "キーボード入力の基本"; }
	}
	public string ClassName
	{
		get { return "Keyboard_Basic"; }
	}

    public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("Keyboard_Basic", 640, 480, new asd.EngineOption());

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

            if(zstate == asd.ButtonState.Free) // Zキーを離している状態。
            {
                    keyStateText.Text = "Zキーを離しています。";
            }
            else if(zstate == asd.ButtonState.Hold) // Zキーを押している状態。
            {
                    keyStateText.Text = "Zキーを押しています。";
            }

            // Altseedを更新する。
            asd.Engine.Update();

            Recorder.CaptureScreen("Keyboard_Basic", 30, 60, 0.5f, 0.5f);
			Recorder.TakeScreenShot("Keyboard_Basic", 30);
        }

        //Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}
