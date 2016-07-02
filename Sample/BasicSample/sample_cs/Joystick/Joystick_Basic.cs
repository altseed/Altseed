/// <summary>
/// ジョイスティックにある全てのボタンの入力状態を取得するサンプル。
/// </summary>
class Joystick_Basic : ISample
{
    public string Description
    {
        get { return "ジョイスティックのボタンの入力状態を調べ、表示するサンプルです。"; }
    }
	public string Title
	{
		get { return "ジョイスティックのボタン入力"; }
	}

    public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("Joystick_Basic", 640, 480, new asd.EngineOption());

        // ジョイスティックの状態を表示するテキストを生成する。
        var font = asd.Engine.Graphics.CreateDynamicFont("", 25, new asd.Color(255, 255, 255, 255), 1, new asd.Color(0, 0, 0, 255));

        // ボタンの入力状態を表示する文字描画オブジェクトを設定して、エンジンに追加する。
        var stateText = new asd.TextObject2D();
        stateText.Position = new asd.Vector2DF(10, 5);
        stateText.Font = font;
        asd.Engine.AddObject2D(stateText);

        //ボタンをたくさん認識する可能性があるため表示の行間を詰める。
        stateText.LineSpacing = -15;

        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            string displayStr = "";

            // ジョイスティックが接続されているかどうかを確認する。
            if (!asd.Engine.JoystickContainer.GetIsPresentAt(0))
            {
                displayStr += "ジョイスティックが接続されていません。";
            }
            else
            {
                // 1つ目のジョイスティックの全てのボタンの入力状態を表示する。
                var joystick = asd.Engine.JoystickContainer.GetJoystickAt(0);

				for (int buttonIndex = 0; buttonIndex < joystick.ButtonsCount; buttonIndex++)
                {
                    var state = joystick.GetButtonState(buttonIndex);

                    if(state == asd.JoystickButtonState.Free) //前フレームと本フレームで非押下
                    {
                            displayStr += ("ボタン " + buttonIndex.ToString() + "を離しています。");
                    }
                    else if(state == asd.JoystickButtonState.Hold) //前フレームと本フレームで押下
                    {
                            displayStr += ("ボタン " + buttonIndex.ToString() + "を押しています。");
                    }
                    else if(state == asd.JoystickButtonState.Release) //前フレームで押下、本フレームで非押下
                    {
                            displayStr += ("ボタン " + buttonIndex.ToString() + "を離しました!");
                    }
                    else if(state == asd.JoystickButtonState.Push) //前フレームで非押下、本フレームで押下
                    {
                            displayStr += ("ボタン " + buttonIndex.ToString() + "を押しました!");
                    }

                    displayStr += "\n";
                }

            }

            stateText.Text = displayStr;

            // Altseedを更新する。
            asd.Engine.Update();

            Recorder.CaptureScreen("Joystick_Basic", 30, 30, 0.2f, 0.7f);
			Recorder.TakeScreenShot("Joystick_Basic", 30);
        }

        //Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}
