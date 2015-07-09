/// <summary>
/// 多角形をテクスチャを合成した上で表示するサンプル。
/// </summary>
class GeometryObject2D_PolygonShape_Textured : ISample
{
    public void Run()
    {
        // Altseedを初期化する
        asd.Engine.Initialize("GeometryObject2D_PolygonShape_Textured", 640, 480, new asd.EngineOption());

        // テクスチャとして図形に合成する画像を読み込む。
        var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");

        // 図形描画オブジェクトのインスタンスを生成する。
        var geometryObj = new asd.GeometryObject2D();

        // 図形描画オブジェクトのインスタンスをエンジンに追加する。
        asd.Engine.AddObject2D(geometryObj);

        // 多角形を図形描画クラスにて描画する。
        var polygon = new asd.PolygonShape();

        // 多角形を構成する頂点を追加していく。（星形になるようにする。）
        for (int i = 0; i < 10; ++i)
        {
            asd.Vector2DF vec = new asd.Vector2DF(1, 0);
            vec.Degree = i * 36;
            vec.Length = (i % 2 == 0) ? 200 : 75;
            polygon.AddVertex(vec + new asd.Vector2DF(320, 240));

        }

        // 多角形を描画する図形として設定し、合成するテクスチャも設定。
        geometryObj.Shape = polygon;
        geometryObj.Texture = texture;

        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            // Altseedを更新する。
            asd.Engine.Update();

            Recorder.TakeScreenShot("GeometryObject2D_PolygonShape_Textured", 30);
        }

        // Altseedを終了する。
        asd.Engine.Terminate();
    }
}
