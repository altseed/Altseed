
/// <summary>
/// Joystickのサンプル。ジョイスティックによる入力を取得しています。
/// </summary>
class Joystick : ISample
{
	public void Run()
	{
		// Altseedを初期化する。
		var initialized = asd.Engine.Initialize("Joystick", 640, 480, new asd.EngineOption());

		asd.JoystickContainer container = asd.Engine.JoystickContainer;

		// 0番目のジョイスティックが接続されているか否かを確かめる
		if (container.GetIsPresentAt(0))
		{
			// 一つも接続されていない場合は終了する
			System.Console.WriteLine("No joystick are connected.");
			asd.Engine.Terminate();
			return;
		}

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// 一つ目のジョイスティックの0番目のボタンの入力状態を表示する
			switch (container.GetJoystickAt(0).GetButtonState(0))
			{
				case asd.JoystickButtonState.Free: //前フレーム、現フレーム共に非押下。
					System.Console.WriteLine("Free");
					break;
				case asd.JoystickButtonState.Hold: //前フレーム、現フレーム共に押下。
					System.Console.WriteLine("Hold");
					break;
				case asd.JoystickButtonState.Release: //前フレームで押下、現フレームで非押下。
					System.Console.WriteLine("Release");
					break;
				case asd.JoystickButtonState.Push: //前フレームで非押下、現フレームで押下。
					System.Console.WriteLine("Push");
					break;
			}

			// Altseedを更新する。
			asd.Engine.Update();
		}
		//Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}

