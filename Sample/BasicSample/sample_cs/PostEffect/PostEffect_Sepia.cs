
class PostEffect_Sepia : ISample
{
	public string Description
	{
		get { return "レイヤーの描画結果をセピア調にするサンプル。"; }
	}
	public string Title
	{
		get { return "セピア調"; }
	}
	public string ClassName
	{
		get { return "PostEffect_Sepia"; }
	}

	// レイヤーにセピアカラーのポストエフェクトを適用する。
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("PostEffect_Sepia", 640, 480, new asd.EngineOption());

		// 画像を読み込む。
		var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Picture1.png");

		// シーンクラスのインスタンスを生成する。
		var scene = new asd.Scene();

		// レイヤークラスのインスタンスを生成する。
		var layer = new asd.Layer2D();

		// 画像描画オブジェクトのインスタンスを生成する。
		var obj = new asd.TextureObject2D();

		// オブジェクトの位置とテクスチャを設定する。
		obj.Position = new asd.Vector2DF(50, 50);
		obj.Texture = texture;

		// シーンを変更し、そのシーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
		asd.Engine.ChangeScene(scene);
		scene.AddLayer(layer);
		layer.AddObject(obj);

		// レイヤーにセピアカラーのポストエフェクトを適用。
		layer.AddPostEffect(new asd.PostEffectSepia());

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