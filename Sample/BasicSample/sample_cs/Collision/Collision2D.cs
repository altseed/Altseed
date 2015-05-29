
/// <summary>
/// Collision2Dのサンプル。マウスによって操作する円がいずれかのオブジェクトにヒットしたら円が赤く変化します。
/// </summary>
public class Collision2D : ISample
{
    public void Run()
    {
        // AC-Engineを初期化する。
        ace.Engine.Initialize("GeometryObject2D", 640, 480, new ace.EngineOption());

        // 図形描画クラスのコンストラクタを呼び出す。
        var geometryObj0 = new ace.GeometryObject2D();
        var geometryObj1 = new ace.GeometryObject2D();
        var geometryObj2 = new ace.GeometryObject2D();
        var geometryObj3 = new ace.GeometryObject2D();

        // マウスによって動かす円。
        ace.CircleShape selfCircle;

        // 停止している円、三角形、矩形。
        ace.CircleShape circle;
        ace.TriangleShape triangle;
        ace.RectangleShape rect;

        // シーンを生成する。
        var scene = new ace.Scene();

        // シーンにレイヤーを追加する。
        var layer = new ace.Layer2D();


        // 図形描画クラスをレイヤーに追加する。
        layer.AddObject(geometryObj0);
        layer.AddObject(geometryObj1);
        layer.AddObject(geometryObj2);
        layer.AddObject(geometryObj3);

        // レイヤーを追加する。
        scene.AddLayer(layer);

        // シーンを切り替える。
        ace.Engine.ChangeScene(scene);

        // マウスによって動かす円の形状と描画の設定を行う。
        {
            // 円の外径、内径、頂点数、中心位置を指定。
            selfCircle = new ace.CircleShape();
            selfCircle.OuterDiameter = 100;
            selfCircle.InnerDiameter = 0;
            selfCircle.NumberOfCorners = 96;
            selfCircle.Position = new ace.Vector2DF(100, 50);

            // 円を描画する図形として最前面に描画されるように設定する。
            geometryObj0.Shape = selfCircle;
            geometryObj0.Position = new ace.Vector2DF(0, 0);
            geometryObj0.DrawingPriority = 1;
        }

        // 停止している円の形状と描画の設定を行う。
        {
            // 円の外径、内径、頂点数、中心位置を指定。
            circle = new ace.CircleShape();
            circle.OuterDiameter = 100;
            circle.InnerDiameter = 0;
            circle.NumberOfCorners = 96;
            circle.Position = new ace.Vector2DF(100, 50);

            // 円を描画する図形として設定する。
            geometryObj1.Shape = circle;
            geometryObj1.Position = new ace.Vector2DF(0, 0);
        }

        // 停止している三角形の形状と描画の設定を行う。
        {
            // 三角形の各頂点の位置を指定します。
            triangle = new ace.TriangleShape();
            triangle.SetPointByIndex(new ace.Vector2DF(400, 350), 0);
            triangle.SetPointByIndex(new ace.Vector2DF(300, 450), 1);
            triangle.SetPointByIndex(new ace.Vector2DF(540, 450), 2);

            // 三角形を描画する図形として設定する。
            geometryObj2.Shape = triangle;
            geometryObj2.Position = new ace.Vector2DF(0, 0);
        }

        // 停止している矩形の形状と描画の設定を行う。
        {
            rect = new ace.RectangleShape();
            rect.DrawingArea = new ace.RectF(10, 110, 300, 200);
            rect.Angle = 15;

            geometryObj3.Shape = rect;
            geometryObj3.Position = new ace.Vector2DF(0, 0);
        }


        // AC-Engineのウインドウが閉じられていないか確認する。
        while (ace.Engine.DoEvents())
        {
            {
                //マウスによって制御される円の中心位置をマウスの位置とする。
                selfCircle.Position = ace.Engine.Mouse.Position;

                //停止している円、三角形、矩形にマウスによって動く円が衝突した時に円を赤く変化させる。
                //そうでない時は白く変化させる。
                if (selfCircle.GetIsCollidedWith(circle)
                    || selfCircle.GetIsCollidedWith(triangle)
                    || selfCircle.GetIsCollidedWith(rect))
                {
                    geometryObj0.Color = new ace.Color(255, 0, 0, 255);
                }
                else
                {
                    geometryObj0.Color = new ace.Color(255, 255, 255, 255);
                }
            }
            // AC-Engineを更新する。
            ace.Engine.Update();
        }

        // AC-Engineの終了処理をする。
        ace.Engine.Terminate();
    }

}
