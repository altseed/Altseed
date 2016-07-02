
class SceneAndLayer_CustomScene : ISample
{
	public string Description
	{
		get { return "Sceneクラスを継承して、ふるまいをカスタマイズするサンプル。"; }
	}
	public string Title
	{
		get { return "シーンのカスタマイズ"; }
	}

	// このサンプルで使用するカスタマイズされたシーン。
	class SampleScene : asd.Scene
	{
		// フレーム数を数えるカウンタ。
		private int count;

		public SampleScene()
		{
			// カウンタを初期化
			count = 0;
		}

		protected override void OnUpdated()
		{
			// フレーム数を数える。
			count++;

			// 60フレームごとにレイヤーを追加する処理
			if (count % 60 == 0)
			{
				// レイヤークラスのインスタンスを生成する。
				var layer = new asd.Layer2D();

				// 描画するレイヤーをシーンに追加する。
				AddLayer(layer);

				// オブジェクトを生成する。
				var obj = new asd.TextureObject2D();

				// オブジェクトに画像を設定する。
				obj.Texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Picture1.png");

				// オブジェクトの位置を設定する。
				var p = count / 60 * 20;
				obj.Position = new asd.Vector2DF(p, p);

				// オブジェクトをレイヤーに追加する。
				layer.AddObject(obj);
			}
		}
	}

	// Sceneクラスを継承することでカスタマイズされたシーンを作り、一定時間ごとにレイヤーを追加していくサンプル。
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("SceneAndLayer_CustomScene", 640, 480, new asd.EngineOption());

		// シーンクラスのインスタンスを生成する。
		var scene = new SampleScene();

		// 描画するシーンを指定する。
		asd.Engine.ChangeScene(scene);

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();
			Recorder.CaptureScreen("SceneAndLayer_CustomScene", 0, 100, 0.5f, 0.5f);
			Recorder.TakeScreenShot("SceneAndLayer_CustomScene", 150);
		}

		// Altseedを終了する。
		asd.Engine.Terminate();
	}
}