
/// <summary>
/// Mouseのサンプル。マウスによる入力を取得しています。
/// </summary>
class Mouse : ISample
{
	public void Run()
	{
		// AC-Engineを初期化する。
		var initialized = ace.Engine.Initialize("Mouse", 640, 480, new ace.EngineOption());

		// AC-Engineが続行可能か調べる。
		while (ace.Engine.DoEvents())
		{
			// マウスへの参照を取得する。
			ace.Mouse mouse = ace.Engine.Mouse;

			//マウスカーソルの座標を取得して表示する。
			{
				ace.Vector2DF v = mouse.Position;

				System.Console.Write("Cursor Position:({0},{1}) ", v.X, v.Y);
			}

			// マウスの中央ボタンの押下状態を表示する。
			switch (mouse.MiddleButton.ButtonState)
			{
				case ace.MouseButtonState.Free: //前フレーム、現フレーム共に非押下。
					System.Console.Write("Middle Free");
					break;
				case ace.MouseButtonState.Hold: //前フレーム、現フレーム共に押下。
					System.Console.Write("Middle Hold");
					break;
				case ace.MouseButtonState.Release: //前フレームで押下、現フレームで非押下。
					System.Console.Write("Middle Release");
					break;
				case ace.MouseButtonState.Push: //前フレームで非押下、現フレームで押下。
					System.Console.Write("Middle Push");
					break;
			}

			// マウスのホイールの回転状態を取得して表示する。
			{
				System.Console.WriteLine(" Wheel : " + mouse.MiddleButton.WheelRotation);
			}

			// AC-Engineを更新する。
			ace.Engine.Update();
		}

		// AC-Engineの終了処理をする。
		ace.Engine.Terminate();
	}
}

