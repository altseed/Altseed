using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.Graphics._2D
{
    /// <summary>
    /// テキストを表示する。
    /// </summary>
    class TextObject2D_Rotate : ISample
    {
        public void Run()
        {
            // aceを初期化する
            asd.Engine.Initialize("TextObject2D_Rotate", 640, 480, new asd.EngineOption());

            {
                var edgeFont = asd.Engine.Graphics.CreateFont("Data/Font/edge.aff");
                var edgeObj = new asd.TextObject2D();
                edgeObj.Font = edgeFont;
                edgeObj.Position = new asd.Vector2DF(100, 100);

                //30度回転させて描画する
                edgeObj.Angle = 30;
                edgeObj.Text = "文字列サンプル2";
                asd.Engine.AddObject2D(edgeObj);
            }
            // aceが進行可能かチェックする。
            while (asd.Engine.DoEvents())
            {
                // aceを更新する。
                asd.Engine.Update();
            }

            // aceを終了する。
            asd.Engine.Terminate();
        }
    }
}