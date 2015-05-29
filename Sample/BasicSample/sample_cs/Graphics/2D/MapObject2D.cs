
/// <summary>
/// MapObject2Dのサンプル。
/// </summary>
class MapObject2D : ISample
{
	public void Run()
	{
		// AC-Engineを初期化する。
		ace.Engine.Initialize("MapObject2D", 640, 480, new ace.EngineOption());

		// シーンを生成する
		var scene = new ace.Scene();

		// レイヤーを生成する
		var layer = new ace.Layer2D();

		// シーンにレイヤーを追加する
		scene.AddLayer(layer);

		// シーンを切り替える
		ace.Engine.ChangeScene(scene);

		//カメラを設定する。
		var camera = new ace.CameraObject2D();

		camera.Src = new ace.RectI(0, 0, 640, 480);
		camera.Dst = new ace.RectI(0, 0, 640, 480);

		{
			//マップオブジェクトを生成する。
			var mapObject = new ace.MapObject2D();

			var texture = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Sample2.png");

			//マップオブジェクトに50*50=2500個のチップを登録する。
			for (int i = 0; i < 50; ++i)
			{
				for (int j = 0; j < 50; ++j)
				{
					//チップを生成する。
					var chip = new ace.Chip2D();

					//チップにテクスチャを設定する。
					chip.Texture = texture;

					//チップの描画先を指定する。
					chip.Position = new ace.Vector2DF(i * 40 - 1000, j * 40 - 1000);

					//マップオブジェクトにチップを追加する。
					mapObject.AddChip(chip);
				}
			}

			//レイヤーにマップオブジェクトを追加する。
			layer.AddObject(mapObject);

			//レイヤーにカメラオブジェクトを追加する。
			layer.AddObject(camera);
		}

		// AC-Engineのウインドウが閉じられていないか確認する。
		while (ace.Engine.DoEvents())
		{
			//カメラを移動させる
			var pos = camera.Src;
			pos.X += 1;
			pos.Y += 1;
			pos.X %= 1000;
			pos.Y %= 1000;
			camera.Src = pos;

			// AC-Engineを更新する。
			ace.Engine.Update();
		}

		// AC-Engineの終了処理をする。
		ace.Engine.Terminate();
	}
}
