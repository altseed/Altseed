/// <summary>
/// Collision2Dのサンプル1。マウスによって操作する円が固定された円にヒットしたら円が赤く変化します。
/// </summary>
public class Collision2D_Basic : ISample
{
    public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("Collision2D_Basic", 640, 480, new asd.EngineOption());

        // 図形描画オブジェクトのインスタンスを生成する。
        asd.GeometryObject2D geometryObj0 = new asd.GeometryObject2D();
        asd.GeometryObject2D geometryObj1 = new asd.GeometryObject2D();

        // マウスによって動かす円。
        asd.CircleShape selfCircle;

        // 停止している円、三角形、矩形。
        asd.CircleShape circle;

        // 図形描画クラスをエンジンに追加する。
        asd.Engine.AddObject2D(geometryObj0);
        asd.Engine.AddObject2D(geometryObj1);

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
            circle.Position = new asd.Vector2DF(100, 100);

            // 円を描画する図形として設定する。
            geometryObj1.Shape = circle;
        }


        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            // マウスによって制御される円の中心位置をマウスの位置とする。
            selfCircle.Position = asd.Engine.Mouse.Position;

            // 固定されている円に、マウスによって動く円が衝突した時に円を赤く変化させる。
            // そうでない時は白く変化させる。
            if (selfCircle.GetIsCollidedWith(circle))
            {
                geometryObj0.Color = new asd.Color(255, 0, 0, 255);
            }
            else
            {
                geometryObj0.Color = new asd.Color(255, 255, 255, 255);
            }

            Recorder.CaptureScreen("Collision2D_Basic", 30, 90, 0.5f, 1);

            // Altseedを更新する。
            asd.Engine.Update();

        }

        // Altseedの終了処理をする。
        asd.Engine.Terminate();

    }
}