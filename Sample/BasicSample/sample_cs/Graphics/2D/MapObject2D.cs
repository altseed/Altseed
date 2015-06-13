
/// <summary>
/// MapObject2Dのサンプル。敷き詰められたタイルをカメラで撮影して画面に表示しています。
/// </summary>
class MapObject2D : ISample
{
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("MapObject2D", 640, 480, new asd.EngineOption());

		// シーンを生成する
		var scene = new asd.Scene();

		// レイヤーを生成する
		var layer = new asd.Layer2D();

		// シーンにレイヤーを追加する
		scene.AddLayer(layer);

		// シーンを切り替える
		asd.Engine.ChangeScene(scene);

		// カメラを設定する。
		var camera = new asd.CameraObject2D();

		camera.Src = new asd.RectI(0, 0, 640, 480);
		camera.Dst = new asd.RectI(0, 0, 640, 480);

		{
			// マップオブジェクトを生成する。
			var mapObject = new asd.MapObject2D();

			var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample2.png");

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

			// レイヤーにマップオブジェクトを追加する。
			layer.AddObject(mapObject);

			// レイヤーにカメラオブジェクトを追加する。
			layer.AddObject(camera);
		}

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// カメラを移動させる
			var pos = camera.Src;
			pos.X += 1;
			pos.Y += 1;
			pos.X %= 1000;
			pos.Y %= 1000;
			camera.Src = pos;

			// Altseedを更新する。
			asd.Engine.Update();
		}

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}
