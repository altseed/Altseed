
/// <summary>
/// Collision2Dのサンプル。マウスによって操作する円がいずれかのオブジェクトにヒットしたら円が赤く変化します。
/// </summary>
public class Collision2D : ISample
{
    public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("GeometryObject2D", 640, 480, new asd.EngineOption());

        // 図形描画クラスのコンストラクタを呼び出す。
        var geometryObj0 = new asd.GeometryObject2D();
        var geometryObj1 = new asd.GeometryObject2D();
        var geometryObj2 = new asd.GeometryObject2D();
        var geometryObj3 = new asd.GeometryObject2D();

        // マウスによって動かす円。
        asd.CircleShape selfCircle;

        // 停止している円、三角形、矩形。
        asd.CircleShape circle;
        asd.TriangleShape triangle;
        asd.RectangleShape rect;

        // シーンを生成する。
        var scene = new asd.Scene();

        // シーンにレイヤーを追加する。
        var layer = new asd.Layer2D();


        // 図形描画クラスをレイヤーに追加する。
        layer.AddObject(geometryObj0);
        layer.AddObject(geometryObj1);
        layer.AddObject(geometryObj2);
        layer.AddObject(geometryObj3);

        // レイヤーを追加する。
        scene.AddLayer(layer);

        // シーンを切り替える。
        asd.Engine.ChangeScene(scene);

        // マウスによって動かす円の形状と描画の設定を行う。
        {
            // 円の外径、内径、頂点数、中心位置を指定。
            selfCircle = new asd.CircleShape();
            selfCircle.OuterDiameter = 100;
            selfCircle.InnerDiameter = 0;
            selfCircle.NumberOfCorners = 96;
            selfCircle.Position = new asd.Vector2DF(100, 50);

            // 円を描画する図形として最前面に描画されるように設定する。
            geometryObj0.Shape = selfCircle;
            geometryObj0.Position = new asd.Vector2DF(0, 0);
            geometryObj0.DrawingPriority = 1;
        }

        // 停止している円の形状と描画の設定を行う。
        {
            // 円の外径、内径、頂点数、中心位置を指定。
            circle = new asd.CircleShape();
            circle.OuterDiameter = 100;
            circle.InnerDiameter = 0;
            circle.NumberOfCorners = 96;
            circle.Position = new asd.Vector2DF(100, 50);

            // 円を描画する図形として設定する。
            geometryObj1.Shape = circle;
            geometryObj1.Position = new asd.Vector2DF(0, 0);
        }

        // 停止している三角形の形状と描画の設定を行う。
        {
            // 三角形の各頂点の位置を指定します。
            triangle = new asd.TriangleShape();
            triangle.SetPointByIndex(new asd.Vector2DF(400, 350), 0);
            triangle.SetPointByIndex(new asd.Vector2DF(300, 450), 1);
            triangle.SetPointByIndex(new asd.Vector2DF(540, 450), 2);

            // 三角形を描画する図形として設定する。
            geometryObj2.Shape = triangle;
            geometryObj2.Position = new asd.Vector2DF(0, 0);
        }

        // 停止している矩形の形状と描画の設定を行う。
        {
            rect = new asd.RectangleShape();
            rect.DrawingArea = new asd.RectF(10, 110, 300, 200);
            rect.Angle = 15;

            geometryObj3.Shape = rect;
            geometryObj3.Position = new asd.Vector2DF(0, 0);
        }


        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            {
                //マウスによって制御される円の中心位置をマウスの位置とする。
                selfCircle.Position = asd.Engine.Mouse.Position;

                //停止している円、三角形、矩形にマウスによって動く円が衝突した時に円を赤く変化させる。
                //そうでない時は白く変化させる。
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
            }
            // Altseedを更新する。
            asd.Engine.Update();
        }

        // Altseedの終了処理をする。
        asd.Engine.Terminate();
    }

}
