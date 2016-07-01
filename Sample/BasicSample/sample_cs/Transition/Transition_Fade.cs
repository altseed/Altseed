

// フェードアウト・フェードインでシーン遷移をするサンプル。

// 遷移後のシーン
class Transition_Fade_Scene2 : asd.Scene
{
	protected override void OnRegistered()
	{
		// 画像を読み込み、オブジェクトに設定する。
		var obj = new asd.TextureObject2D();
		var tex = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Scene2.png");
		obj.Texture = tex;

		// シーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
		var layer = new asd.Layer2D();
		AddLayer(layer);
		layer.AddObject(obj);
	}
}

// 遷移前のシーン
class Transition_Fade_Scene1 : asd.Scene
{
	protected override void OnRegistered()
	{
		// 画像を読み込み、オブジェクトに設定する。
		var obj = new asd.TextureObject2D();
		var tex = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Scene1.png");
		obj.Texture = tex;

		// シーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
		var layer = new asd.Layer2D();
		AddLayer(layer);
		layer.AddObject(obj);
	}

	protected override void OnUpdated()
	{
		// マウスの左ボタンが押されるのを待つ。
		if (asd.Engine.Mouse.LeftButton.ButtonState == asd.MouseButtonState.Push)
		{
			// フェードアウト・インによるシーン遷移を開始する。
			// 1秒かけてフェードアウトし、1.5秒かけてフェードイン。
			asd.Engine.ChangeSceneWithTransition(new Transition_Fade_Scene2(), new asd.TransitionFade(1.0f, 1.5f));
		}
	}
}

class Transition_Fade : ISample
{
    public string Description
    {
        get { return ""; }
    }
	public string Title
	{
		get { return ""; }
	}

	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("Transition_Fade", 640, 480, new asd.EngineOption());

		// シーンのインスタンスを生成する。
		var scene = new Transition_Fade_Scene1();

		// シーンをシーン1に設定する。
		asd.Engine.ChangeScene(scene);

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();
		}

		// Altseedを終了する。
		asd.Engine.Terminate();
	}
}

