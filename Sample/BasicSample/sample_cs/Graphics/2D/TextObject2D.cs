
/// <summary>
/// TextObject2Dのサンプル。TextObject2Dを用いて文字を表示しています。
/// </summary>
class TextObject2D : ISample
{
	public void Run()
	{
		// AC-Engineを初期化する。
		ace.Engine.Initialize("TextObject2D", 640, 480, new ace.EngineOption());

		// シーンを生成する
		var scene = new ace.Scene();

		// レイヤーを生成する
		var layer = new ace.Layer2D();

		// シーンにレイヤーを追加する
		scene.AddLayer(layer);

		// シーンを切り替える
		ace.Engine.ChangeScene(scene);

		// フォントを生成する
		var smallFont = ace.Engine.Graphics.CreateFont("Data/Font/small.aff");

		// TextObject2Dのインスタンスを生成する
		var fontObj = new ace.TextObject2D();

		// 描画に使うフォントを設定する
		fontObj.Font = smallFont;

		// 描画位置を指定する
		fontObj.Position = new ace.Vector2DF(100, 100);

		// 描画する文字列を指定する
		fontObj.Text = "文字列サンプル1";

		//レイヤーに追加する
		layer.AddObject(fontObj);
		

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
