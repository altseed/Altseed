
/// <summary>
/// // TextureObject2Dのサンプル。TextureObject2Dを用いて画像を表示しています。
/// </summary>
class TextureObject2D : ISample
{
	public void Run()
	{
		// AC-Engineを初期化する。
		ace.Engine.Initialize("TextureObject2D", 640, 480, new ace.EngineOption());

		// シーンを生成する
		var scene = new ace.Scene();

		// レイヤーを生成する
		var layer = new ace.Layer2D();

		// シーンにレイヤーを追加する
		scene.AddLayer(layer);

		// シーンを切り替える
		ace.Engine.ChangeScene(scene);

		// テクスチャを読み込む
		var tex = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");

		// TextureObject2Dのインスタンスを生成する
		var obj = new ace.TextureObject2D();

		// 描画に使うテクスチャを設定する
		obj.Texture = tex;

		// 描画位置を指定する
		obj.Position = new ace.Vector2DF(100, 100);

		// レイヤーに追加する
		layer.AddObject(obj);

		// AC-Engineのウインドウが閉じられていないか確認する。
		while (ace.Engine.DoEvents())
		{
			// AC-Engineを更新する。
			ace.Engine.Update();
		}

		// AC-Engineの終了処理をする。
		ace.Engine.Terminate();
	}
}
