using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// <summary>
/// 文字列を回転させて表示するサンプル。
/// </summary>
class TextObject2D_Rotate : ISample
{
    public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("TextObject2D_Rotate", 640, 480, new asd.EngineOption());

        {
            var edgeFont = asd.Engine.Graphics.CreateFont("Data/Font/edge.aff");
            var edgeObj = new asd.TextObject2D();
            edgeObj.Font = edgeFont;
            edgeObj.Position = new asd.Vector2DF(100, 100);

            // 30度回転させて描画する
            edgeObj.Angle = 30;
            edgeObj.Text = "文字列サンプル2";
            asd.Engine.AddObject2D(edgeObj);
        }
        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            // Altseedを更新する。
            asd.Engine.Update();
        }

        // Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}