using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// <summary>
/// 文字列を表示するサンプル。
/// </summary>
class TextObject2D_Basic : ISample
{
    public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("TextObject2D_Basic", 640, 480, new asd.EngineOption());

        {
            // フォントを生成する
            var font = asd.Engine.Graphics.CreateFont("Data/Font/Font1.aff");

            // TextObject2Dを生成する
            var obj = new asd.TextObject2D();

            // 描画に使うフォントを設定する
            obj.Font = font;

            // 描画位置を指定する
            obj.Position = new asd.Vector2DF(100, 100);

            // 描画する文字列の指定
            obj.Text = "普通の文字列描画";

            // エンジンへの追加
            asd.Engine.AddObject2D(obj);
        }

        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            // Altseedを更新する。
            asd.Engine.Update();
            Recorder.TakeScreenShot("TextObject2D_Basic", 20);
        }

        // Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}