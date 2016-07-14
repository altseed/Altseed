/// <summary>
/// 弧を表示するサンプル。
/// </summary>
class GeometryObject2D_ArcShape : ISample
{
    public string Description
    {
        get { return ""; }
    }
	public string Title
	{
		get { return ""; }
	}
	public string ClassName
	{
		get { return "GeometryObject2D_ArcShape"; }
	}

    public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("GeometryObject2D_ArcShape", 640, 480, new asd.EngineOption());

        // 図形描画クラスのコンストラクタを呼び出す。
        var geometryObj = new asd.GeometryObject2D();

        // 図形描画オブジェクトのインスタンスをエンジンに追加する。
        asd.Engine.AddObject2D(geometryObj);

        // 弧の図形クラスのインスタンスを生成する。
        var arc = new asd.ArcShape();

        // 弧の外径、内径、頂点数、中心位置、開始頂点番号、終了頂点番号を指定する。
        arc.OuterDiameter = 400;
        arc.InnerDiameter = 40;
        arc.NumberOfCorners = 96;
        arc.Position = new asd.Vector2DF(320, 240);
        arc.StartingCorner = 90;
        arc.EndingCorner = 45;

        // 弧を描画する図形として設定する。
        geometryObj.Shape = arc;

        // Altseedを更新する。
        while (asd.Engine.DoEvents())
        {
            // Altseedを更新する。
            asd.Engine.Update();

            Recorder.TakeScreenShot("GeometryObject2D_ArcShape", 30);
        }

        // Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}
