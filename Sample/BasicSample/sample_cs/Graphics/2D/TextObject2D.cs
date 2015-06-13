
/// <summary>
/// TextObject2Dのサンプル。TextObject2Dを用いて文字を表示しています。
/// </summary>
class TextObject2D : ISample
{
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("TextObject2D", 640, 480, new asd.EngineOption());

		// シーンを生成する
		var scene = new asd.Scene();

		// レイヤーを生成する
		var layer = new asd.Layer2D();

		// シーンにレイヤーを追加する
		scene.AddLayer(layer);

		// シーンを切り替える
		asd.Engine.ChangeScene(scene);

		// フォントを生成する
		var smallFont = asd.Engine.Graphics.CreateFont("Data/Font/small.aff");

		// TextObject2Dのインスタンスを生成する
		var fontObj = new asd.TextObject2D();

		// 描画に使うフォントを設定する
		fontObj.Font = smallFont;

		// 描画位置を指定する
		fontObj.Position = new asd.Vector2DF(100, 100);

		// 描画する文字列を指定する
		fontObj.Text = "文字列サンプル1";

		//レイヤーに追加する
		layer.AddObject(fontObj);
		

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
