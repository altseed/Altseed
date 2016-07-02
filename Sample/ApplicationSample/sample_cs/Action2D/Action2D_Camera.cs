
class Action2D_Camera : ISample
{
	public string Description
	{
		get { return "アクションゲームでカメラがキャラクターを追うサンプルです。"; }
	}

	public string Title
	{
		get { return "アクションゲームのカメラ"; }
	}

	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("Action2D_Camera", 640, 480, new asd.EngineOption());

		// カメラを設定する。
		var camera = new asd.CameraObject2D();

		camera.Src = new asd.RectI(0, 0, 640, 480);
		camera.Dst = new asd.RectI(0, 0, 640, 480);

		// エンジンにカメラオブジェクトを追加する。
		asd.Engine.AddObject2D(camera);

		{
			// マップオブジェクトを生成する。
			var mapObject = new asd.MapObject2D();
		
			var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Chip1.png");
		
			// マップオブジェクトに50*50=2500個のチップを登録する。
			for (int i = 0; i < 50; ++i)
			{
				for (int j = 0; j < 50; ++j)
				{
					// チップを生成する。
					var chip = new asd.Chip2D();
		
					// チップにテクスチャを設定する。
					chip.Texture = texture;
		
					// チップの描画先を指定する。
					chip.Position = new asd.Vector2DF(i * 40 - 1000, j * 40 - 1000);
		
					// マップオブジェクトにチップを追加する。
					mapObject.AddChip(chip);
				}
			}
		
			// マップオブジェクトのインスタンスをエンジンへ追加する。
			asd.Engine.AddObject2D(mapObject);
		}

		// キャラクターを設定する。
		var charactor = new asd.TextureObject2D();

		// キャラクターの画像を読み込む。
		charactor.Texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Character1.png");

		// キャラクターをエンジンに追加する。
		asd.Engine.AddObject2D(charactor);

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// キャラクターを移動させる。
			if (asd.Engine.Keyboard.GetKeyState(asd.Keys.Up) == asd.KeyState.Hold) charactor.Position = charactor.Position + new asd.Vector2DF(0, -2);
			if (asd.Engine.Keyboard.GetKeyState(asd.Keys.Down) == asd.KeyState.Hold) charactor.Position = charactor.Position + new asd.Vector2DF(0, +2);
			if (asd.Engine.Keyboard.GetKeyState(asd.Keys.Left) == asd.KeyState.Hold) charactor.Position = charactor.Position + new asd.Vector2DF(-2, 0);
			if (asd.Engine.Keyboard.GetKeyState(asd.Keys.Right) == asd.KeyState.Hold) charactor.Position = charactor.Position + new asd.Vector2DF(+2, 0);

			//カメラをキャラクターの位置に合わせる。
			var pos = camera.Src;
			pos.X = (int)charactor.Position.X - 640 / 2;
			pos.Y = (int)charactor.Position.Y - 480 / 2;
			camera.Src = pos;

			// Altseedを更新する。
			asd.Engine.Update();

			Recorder.TakeScreenShot("Action2D_Camera", 20);
		}

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}

