
/// <summary>
/// Mouseのサンプル。マウスによる入力を取得しています。
/// </summary>
class Mouse : ISample
{
	public void Run()
	{
		// Altseedを初期化する。
		var initialized = asd.Engine.Initialize("Mouse", 640, 480, new asd.EngineOption());

		// Altseedが続行可能か調べる。
		while (asd.Engine.DoEvents())
		{
			// マウスへの参照を取得する。
			asd.Mouse mouse = asd.Engine.Mouse;

			//マウスカーソルの座標を取得して表示する。
			{
				asd.Vector2DF v = mouse.Position;

				System.Console.Write("Cursor Position:({0},{1}) ", v.X, v.Y);
			}

			// マウスの中央ボタンの押下状態を表示する。
			switch (mouse.MiddleButton.ButtonState)
			{
				case asd.MouseButtonState.Free: //前フレーム、現フレーム共に非押下。
					System.Console.Write("Middle Free");
					break;
				case asd.MouseButtonState.Hold: //前フレーム、現フレーム共に押下。
					System.Console.Write("Middle Hold");
					break;
				case asd.MouseButtonState.Release: //前フレームで押下、現フレームで非押下。
					System.Console.Write("Middle Release");
					break;
				case asd.MouseButtonState.Push: //前フレームで非押下、現フレームで押下。
					System.Console.Write("Middle Push");
					break;
			}

			// マウスのホイールの回転状態を取得して表示する。
			{
				System.Console.WriteLine(" Wheel : " + mouse.MiddleButton.WheelRotation);
			}

			// Altseedを更新する。
			asd.Engine.Update();
		}

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}

