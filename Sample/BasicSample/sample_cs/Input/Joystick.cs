
/// <summary>
/// Joystickのサンプル。ジョイスティックによる入力を取得しています。
/// </summary>
class Joystick : ISample
{
	public void Run()
	{
		// AC-Engineを初期化する。
		var initialized = ace.Engine.Initialize("Joystick", 640, 480, new ace.EngineOption());

		ace.JoystickContainer container = ace.Engine.JoystickContainer;

		// 0番目のジョイスティックが接続されているか否かを確かめる
		if (container.GetIsPresentAt(0))
		{
			// 一つも接続されていない場合は終了する
			System.Console.WriteLine("No joystick are connected.");
			ace.Engine.Terminate();
			return;
		}

		// AC-Engineのウインドウが閉じられていないか確認する。
		while (ace.Engine.DoEvents())
		{
			// 一つ目のジョイスティックの0番目のボタンの入力状態を表示する
			switch (container.GetJoystickAt(0).GetButtonState(0))
			{
				case ace.JoystickButtonState.Free: //前フレーム、現フレーム共に非押下。
					System.Console.WriteLine("Free");
					break;
				case ace.JoystickButtonState.Hold: //前フレーム、現フレーム共に押下。
					System.Console.WriteLine("Hold");
					break;
				case ace.JoystickButtonState.Release: //前フレームで押下、現フレームで非押下。
					System.Console.WriteLine("Release");
					break;
				case ace.JoystickButtonState.Push: //前フレームで非押下、現フレームで押下。
					System.Console.WriteLine("Push");
					break;
			}

			// AC-Engineを更新する。
			ace.Engine.Update();
		}
		//AC-Engineの終了処理をする。
		ace.Engine.Terminate();
	}
}

