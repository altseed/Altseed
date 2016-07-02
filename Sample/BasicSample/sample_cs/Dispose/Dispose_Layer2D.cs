
// Disposeが呼ばれた時や破棄されたときにコンソールに出力するオブジェクト。
class Dispose_Layer2D_MessageObject : asd.TextureObject2D
{
	// Disposeが呼ばれたときに呼ばれる。
	protected override void OnDispose()
	{
		System.Console.WriteLine("MessageObject.OnDispose");
	}
}

// Disposeが呼ばれた時や破棄されたときにコンソールに出力するレイヤー。
class Dispose_Layer2D_MessageLayer : asd.Layer2D
{
	// Disposeが呼ばれたときに呼ばれる。
	protected override void OnDispose()
	{
		System.Console.WriteLine("MessageLayer.OnDispose");
	}
}

class Dispose_Layer2D : ISample
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
		asd.Engine.Initialize("Dispose_Object2D", 640, 480, new asd.EngineOption());

		// 画像を読み込む。
		var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Picture1.png");

		// シーンのインスタンスを生成する。
		var scene = new asd.Scene();

		// カスタマイズしたレイヤーのインスタンスを生成する。
		var layer = new Dispose_Layer2D_MessageLayer();

		// カスタマイズしたオブジェクトのインスタンスを生成する。
		var obj = new Dispose_Layer2D_MessageObject();

		// オブジェクトの位置とテクスチャを設定する。
		obj.Position = new asd.Vector2DF(50, 50);
		obj.Texture = texture;

		// 描画するシーンを指定する。
		asd.Engine.ChangeScene(scene);

		// 描画するレイヤーをシーンに追加する。
		scene.AddLayer(layer);

		// 描画するオブジェクトをレイヤーに追加する。
		layer.AddObject(obj);

		// フレーム数を数えるための変数。
		int count = 0;

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();

			// フレームを数える。
			count++;

			// 10フレーム目になったら
			if (count == 10)
			{
				System.Console.WriteLine("Layerを破棄します");
				// レイヤーを破棄する。
				layer.Dispose();
			}
		}

		// Altseedを終了する。
		asd.Engine.Terminate();
	}
}