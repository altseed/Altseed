using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// <summary>
/// 矩形を表示するサンプル。
/// </summary>
class GeometryObject2D_RectangleShape : ISample
{
    public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("GeometryObject2D_RectangleShape", 640, 480, new asd.EngineOption());

        // 図形描画オブジェクトのインスタンスを生成する。
        var geometryObj = new asd.GeometryObject2D();

        // 図形描画クラスのインスタンスをエンジンに追加する。
        asd.Engine.AddObject2D(geometryObj);

        // 矩形の図形クラスのインスタンスを生成する。
        var rect = new asd.RectangleShape();

        // 矩形の描画範囲を指定する。
        rect.DrawingArea = new asd.RectF(10, 210, 300, 200);

        // 矩形を描画する図形として設定しする。
        geometryObj.Shape = rect;

        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            // Altseedを更新する。
            asd.Engine.Update();

            Recorder.TakeScreenShot("GeometryObject2D_RectangleShape", 30);
        }

        // Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}
