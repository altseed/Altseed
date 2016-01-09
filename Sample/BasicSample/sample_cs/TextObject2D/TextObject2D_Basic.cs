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
    public string Description
    {
        get { return ""; }
    }

    public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("TextObject2D_Basic", 640, 480, new asd.EngineOption());

        {
            // フォントを生成する。
            var font = asd.Engine.Graphics.CreateFont("Data/Font/Font1.aff");

            // 文字描画オブジェクトを生成する。
            var obj = new asd.TextObject2D();

            // 描画に使うフォントを設定する。
            obj.Font = font;

            // 描画位置を指定する。
            obj.Position = new asd.Vector2DF(100, 100);

            // 描画する文字列を指定する。
            obj.Text = "普通の文字列描画";

            // 文字描画オブジェクトのインスタンスをエンジンへ追加する。
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