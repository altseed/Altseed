/// <summary>
/// 一部のゲームパッドから入力状態を取得するサンプル。
/// </summary>
class Joystick_GamePad : ISample
{
	public string Description
	{
		get { return "一部のゲームパッドの入力状態を調べ、表示するサンプルです。"; }
	}
	public string Title
	{
		get { return "ゲームパッド入力"; }
	}
	public string ClassName
	{
		get { return "Joystick_GamePad"; }
	}

	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("Joystick_GamePad", 640, 480, new asd.EngineOption());

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
			else if (asd.Engine.JoystickContainer.GetJoystickAt(0).JoystickType == asd.JoystickType.Other)
			{
				displayStr += "認識できないジョイステイックです。";
			}
			else
			{
				// 1つ目のジョイスティックのボタンの入力状態を表示する。
				// RightRightはPSの場合〇、XBOXの場合、Bボタンを示す。
				var joystick = asd.Engine.JoystickContainer.GetJoystickAt(0);

				var state = joystick.GetButtonStateAt(asd.JoystickButtonType.RightRight);
				if (state == asd.JoystickButtonState.Free) //前フレームと本フレームで非押下
				{
					displayStr += ("ボタンを離しています。");
				}
				else if (state == asd.JoystickButtonState.Hold) //前フレームと本フレームで押下
				{
					displayStr += ("ボタンを押しています。");
				}
				else if (state == asd.JoystickButtonState.Release) //前フレームで押下、本フレームで非押下
				{
					displayStr += ("ボタンを離しました!");
				}
				else if (state == asd.JoystickButtonState.Push) //前フレームで非押下、本フレームで押下
				{
					displayStr += ("ボタンを押しました!");
				}

			}

			stateText.Text = displayStr;

			// Altseedを更新する。
			asd.Engine.Update();

			Recorder.CaptureScreen("Joystick_GamePad", 30, 30, 0.2f, 0.7f);
			Recorder.TakeScreenShot("Joystick_GamePad", 30);
		}

		//Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}
