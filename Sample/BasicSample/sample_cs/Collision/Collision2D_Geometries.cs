/// <summary>
/// Collision2Dのサンプル。マウスによって操作する円が固定されたいずれかのオブジェクトにヒットしたら円が赤く変化します。
/// </summary>
public class Collision2D_Geometries : ISample
{
    public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("Collision2D_Geometries", 640, 480, new asd.EngineOption());

        // 図形描画オブジェクトのインスタンスを生成する。
        asd.GeometryObject2D geometryObj0 = new asd.GeometryObject2D();
        asd.GeometryObject2D geometryObj1 = new asd.GeometryObject2D();
        asd.GeometryObject2D geometryObj2 = new asd.GeometryObject2D();
        asd.GeometryObject2D geometryObj3 = new asd.GeometryObject2D();

        // マウスによって動かす円。
        asd.CircleShape selfCircle;

        // 停止している円、三角形、矩形。
        asd.CircleShape circle;
        asd.TriangleShape triangle;
        asd.RectangleShape rect;


        // 図形描画クラスをエンジンに追加する。
        asd.Engine.AddObject2D(geometryObj0);
        asd.Engine.AddObject2D(geometryObj1);
        asd.Engine.AddObject2D(geometryObj2);
        asd.Engine.AddObject2D(geometryObj3);

        // マウスによって動かす円の形状と描画の設定を行う。
        {
            // 円の外径、内径、頂点数、中心位置を指定する。
            selfCircle = new asd.CircleShape();
            selfCircle.OuterDiameter = 100;

            // 円を描画する図形として最前面に描画されるように設定する。
            geometryObj0.Shape = selfCircle;
        }

        // 停止している円の形状と描画の設定を行う。
        {
            // 円の外径、内径、頂点数、中心位置を指定する。
            circle = new asd.CircleShape();
            circle.OuterDiameter = 100;
            circle.Position = new asd.Vector2DF(400, 100);

            // 円を描画する図形として設定する。
            geometryObj1.Shape = circle;
        }

        // 停止している三角形の形状と描画の設定を行う。
        {
            // 三角形の各頂点の位置を指定します。
            triangle = new asd.TriangleShape();
            triangle.SetPointByIndex(new asd.Vector2DF(440, 350), 0);
            triangle.SetPointByIndex(new asd.Vector2DF(340, 450), 1);
            triangle.SetPointByIndex(new asd.Vector2DF(580, 450), 2);

            // 三角形を描画する図形として設定する。
            geometryObj2.Shape = triangle;
        }

        // 停止している矩形の形状と描画の設定を行う。
        {
            rect = new asd.RectangleShape();
            rect.DrawingArea = new asd.RectF(70, 110, 300, 200);
            rect.Angle = 15;

            geometryObj3.Shape = rect;
        }


        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            // マウスによって制御される円の中心位置をマウスの位置とする。
            selfCircle.Position = asd.Engine.Mouse.Position;

            // 停止している円・三角形・矩形のいずれかに、マウスによって動く円が衝突した時に円を赤く変化させる。
            // そうでない時は白く変化させる。
            if (selfCircle.GetIsCollidedWith(circle)
                || selfCircle.GetIsCollidedWith(triangle)
                || selfCircle.GetIsCollidedWith(rect))
            {
                geometryObj0.Color = new asd.Color(255, 0, 0, 255);
            }
            else
            {
                geometryObj0.Color = new asd.Color(255, 255, 255, 255);
            }

            Recorder.CaptureScreen("Collision2D_Geometries", 30, 120, 0.5f, 1);

            // Altseedを更新する。
            asd.Engine.Update();
        }

        // Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}
