
/// <summary>
/// 文字列を中央揃えで描画するサンプル
/// </summary>
class TextObject2D_Centering : ISample
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
            //Altseedを初期化する。
            asd.Engine.Initialize("TextObject2D_Centering", 640, 480, new asd.EngineOption());

            //フォントを生成する。
            var font = asd.Engine.Graphics.CreateDynamicFont(string.Empty, 40, new asd.Color(255, 0, 0, 255), 1, new asd.Color(255, 255, 255, 255));

            // 文字描画オブジェクトを生成する。
            var obj = new asd.TextObject2D();

            // 描画に使うフォントを設定する。
            obj.Font = font;

            //描画する文字列を設定する。
            var str = "中央揃えサンプル";
            obj.Text = str;

            //文字列を描画したときの縦横の大きさを取得する。
            var size = font.CalcTextureSize(str, asd.WritingDirection.Horizontal);

            // 描画位置を指定する。（画面中心が基準）
            var pos = new asd.Vector2DF(asd.Engine.WindowSize.X - size.X, asd.Engine.WindowSize.Y - size.Y);
            obj.Position = pos / 2;

            // 文字描画オブジェクトのインスタンスをエンジンへ追加する。
            asd.Engine.AddObject2D(obj);

            // Altseedのウインドウが閉じられていないか確認する。
            while (asd.Engine.DoEvents())
            {
                // Altseedを更新する。
                asd.Engine.Update();
				Recorder.TakeScreenShot("TextObject2D_Centering", 20);
            }

            //Altseedの終了処理をする。
            asd.Engine.Terminate();
    }
}
