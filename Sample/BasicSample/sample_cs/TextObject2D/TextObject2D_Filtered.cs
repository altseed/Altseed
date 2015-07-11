using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// <summary>
/// 文字列をギザギザを防いで回転させて表示するサンプル。
/// </summary>
class TextObject2D_Filtered : ISample
{
    public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("TextObject2D_Filter", 640, 480, new asd.EngineOption());


        {
            // フォントと文字列描画オブジェクトの設定を行う。
            var edgeFont = asd.Engine.Graphics.CreateFont("Data/Font/Font1.aff");
            var edgeObj = new asd.TextObject2D();
            edgeObj.Font = edgeFont;
            edgeObj.Position = new asd.Vector2DF(100, 100);

            // 30度回転させて描画する
            edgeObj.Angle = 30;

            // 描画のフィルタを線形補間にすることによって、描画時に境界がギザギザにならないようにする。
            edgeObj.TextureFilterType = asd.TextureFilterType.Linear;

            edgeObj.Text = "ギザギザをなくした文字列の描画";
            asd.Engine.AddObject2D(edgeObj);
        }
        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            // Altseedを更新する。
            asd.Engine.Update();
            Recorder.TakeScreenShot("TextObject2D_Filtered", 20);
        }

        // Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}