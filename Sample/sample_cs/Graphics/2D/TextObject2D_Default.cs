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
    class TextObject2D_Default : ISample
    {
        public void Run()
        {
            // aceを初期化する
            asd.Engine.Initialize("TextObject2D_Default", 640, 480, new asd.EngineOption());

            {
                // フォントを生成する
                var font = asd.Engine.Graphics.CreateFont("Data/Font/edge.aff");

                //TextObject2Dを生成する
                var obj = new asd.TextObject2D();

                //描画に使うフォントを設定する
                obj.Font = font;

                //描画位置を指定する
                obj.Position = new asd.Vector2DF(100, 100);

                //描画する文字列の指定
                obj.Text = "文字列サンプル1";

                //レイヤーへの追加
                asd.Engine.AddObject2D(obj);
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