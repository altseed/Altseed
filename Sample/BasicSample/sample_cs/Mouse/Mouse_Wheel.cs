/// <summary>
/// マウスのホイールの回転度合を取得するサンプル。
/// </summary>
public class Mouse_Wheel : ISample
{
    public string Description
    {
        get { return "マウスホイールの回転の度合いを調べ、表示するサンプルです。"; }
    }
	public string Title
	{
		get { return "マウスホイール"; }
	}

    public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("Mouse_Wheel", 640, 480, new asd.EngineOption());

        // マウスの状態を表示するテキストを生成する。
        var font = asd.Engine.Graphics.CreateDynamicFont("", 40, new asd.Color(255, 255, 255, 255), 1, new asd.Color(0, 0, 0, 255));

        // マウスのホイールの回転度合を表示する文字描画オブジェクトを設定して、エンジンに追加する。
        var wheelStateText = new asd.TextObject2D();
        wheelStateText.Position = new asd.Vector2DF(10, 10);
        wheelStateText.Font = font;
        asd.Engine.AddObject2D(wheelStateText);

        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            // マウスのホイールの回転状態を取得して表示する。
            wheelStateText.Text = "ホイールの回転度合 : " + asd.Engine.Mouse.MiddleButton.WheelRotation.ToString();

            // Altseedを更新する。
            asd.Engine.Update();

            Recorder.CaptureScreen("Mouse_Wheel", 30, 120, 0.5f, 0.5f);
			Recorder.TakeScreenShot("Mouse_Wheel", 30);
        }

        //Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}
