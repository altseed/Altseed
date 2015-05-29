
/// <summary>
/// Keyboardのサンプル。キーボードによる入力を取得しています。
/// </summary>
class Keyboard : ISample
{
	public void Run()
	{
		//AC-Engineを初期化する。
		var initialized = ace.Engine.Initialize("Keyboard", 640, 480, new ace.EngineOption());

		// AC-Engineのウインドウが閉じられていないか確認する。
		while (ace.Engine.DoEvents())
		{
			//キーボードへの参照を取得する。
			ace.Keyboard keyboard = ace.Engine.Keyboard;

			//Zキーの押下状態を表示する。
			switch (keyboard.GetKeyState(ace.Keys.Z))
			{
				case ace.KeyState.Free: //前フレーム、現フレーム共に非押下。
					System.Console.WriteLine("Z Free");
					break;
				case ace.KeyState.Hold: //前フレーム、現フレーム共に押下。
					System.Console.WriteLine("Z Hold");
					break;
				case ace.KeyState.Release: //前フレームで押下、現フレームで非押下。
					System.Console.WriteLine("Z Release");
					break;
				case ace.KeyState.Push: //前フレームで非押下、現フレームで押下。
					System.Console.WriteLine("Z Push");
					break;
			}

			// AC-Engineを更新する。
			ace.Engine.Update();
		}

		//AC-Engineの終了処理をする。
		ace.Engine.Terminate();
	}
}
