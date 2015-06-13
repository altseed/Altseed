
/// <summary>
/// // TextureObject2Dのサンプル。TextureObject2Dを用いて画像を表示しています。
/// </summary>
class TextureObject2D : ISample
{
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("TextureObject2D", 640, 480, new asd.EngineOption());

		// シーンを生成する
		var scene = new asd.Scene();

		// レイヤーを生成する
		var layer = new asd.Layer2D();

		// シーンにレイヤーを追加する
		scene.AddLayer(layer);

		// シーンを切り替える
		asd.Engine.ChangeScene(scene);

		// テクスチャを読み込む
		var tex = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");

		// TextureObject2Dのインスタンスを生成する
		var obj = new asd.TextureObject2D();

		// 描画に使うテクスチャを設定する
		obj.Texture = tex;

		// 描画位置を指定する
		obj.Position = new asd.Vector2DF(100, 100);

		// レイヤーに追加する
		layer.AddObject(obj);

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();
		}

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}
