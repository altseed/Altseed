
/// <summary>
/// ポーズを実行するレイヤー
/// </summary>
class Pause_PauseLayer : asd.Layer2D
{
	protected override void OnAdded()
	{
		// ポーズ以外のレイヤーの更新を停止する。
		foreach(var layer in Scene.Layers)
		{
			if(layer != this) layer.IsUpdated = false;
		}

		// ポーズを表示するオブジェクトを生成する。
		var obj = new asd.TextureObject2D();
		obj.Texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Pause1.png");
		AddObject(obj);
	}

	protected override void OnUpdated()
	{
		// スペースが押されたら、他のレイヤーの更新を再開して、ポーズレイヤーを破棄する。
		if(asd.Engine.Keyboard.GetKeyState(asd.Keys.Space) == asd.KeyState.Push)
		{
			foreach(var layer in Scene.Layers)
			{
				layer.IsUpdated = true;
			}
			Dispose();
		}
	}
}

/// <summary>
/// ゲームの挙動を描画するレイヤー
/// </summary>
class Pause_MainLayer : asd.Layer2D
{
	asd.TextureObject2D obj = null;

	public Pause_MainLayer()
	{
		// 画像を表示するオブジェクトを生成する。
		obj = new asd.TextureObject2D();
		obj.Texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Picture1.png");
		obj.Position = new asd.Vector2DF(320, 240);
		obj.CenterPosition = new asd.Vector2DF(obj.Texture.Size.X / 2, obj.Texture.Size.Y / 2);
		AddObject(obj);
	}

	protected override void OnUpdated()
	{
		// スペースが押されたら、ポーズレイヤーを追加する。
		if(asd.Engine.Keyboard.GetKeyState(asd.Keys.Space) == asd.KeyState.Push)
		{
			var pauseLayer = new Pause_PauseLayer();
			Scene.AddLayer(pauseLayer);
		}

		// 画像を回転させる。
		obj.Angle += 1.0f;
	}
}

/// <summary>
/// ポーズのサンプル。
/// </summary>
class Pause_Basic : ISample
{
	public string Description
	{
		get { return "レイヤーの処理を一時停止するサンプルです。"; }
	}

	public string Title
	{
		get { return "ポーズ"; }
	}

	public string ClassName
	{
		get { return "Pause_Basic"; }
	}

	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("Pause_Basic", 640, 480, new asd.EngineOption());

		// シーンを生成する
		var scene = new asd.Scene();

		// ゲームの挙動を描画するレイヤーを生成する
		var layer = new Pause_MainLayer();

		// シーンにレイヤーを追加する
		scene.AddLayer(layer);

		// シーンを切り替える
		asd.Engine.ChangeScene(scene);

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();
			Recorder.TakeScreenShot("Pause_Basic", 60);
		}

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}
