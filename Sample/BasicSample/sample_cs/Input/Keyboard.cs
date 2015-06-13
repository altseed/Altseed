
/// <summary>
/// Keyboardのサンプル。キーボードによる入力を取得しています。
/// </summary>
class Keyboard : ISample
{
	public void Run()
	{
		//Altseedを初期化する。
		var initialized = asd.Engine.Initialize("Keyboard", 640, 480, new asd.EngineOption());

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			//キーボードへの参照を取得する。
			asd.Keyboard keyboard = asd.Engine.Keyboard;

			//Zキーの押下状態を表示する。
			switch (keyboard.GetKeyState(asd.Keys.Z))
			{
				case asd.KeyState.Free: //前フレーム、現フレーム共に非押下。
					System.Console.WriteLine("Z Free");
					break;
				case asd.KeyState.Hold: //前フレーム、現フレーム共に押下。
					System.Console.WriteLine("Z Hold");
					break;
				case asd.KeyState.Release: //前フレームで押下、現フレームで非押下。
					System.Console.WriteLine("Z Release");
					break;
				case asd.KeyState.Push: //前フレームで非押下、現フレームで押下。
					System.Console.WriteLine("Z Push");
					break;
			}

			// Altseedを更新する。
			asd.Engine.Update();
		}

		//Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}
