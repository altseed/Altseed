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
    class TextObject2D : ISample
    {
        public void Run()
        {
            // aceを初期化する
            asd.Engine.Initialize("TextObject2D", 640, 480, new asd.EngineOption());

            // シーンを生成する
            var scene = new asd.Scene();

            // レイヤーを生成する
            var layer = new asd.Layer2D();

            // シーンにレイヤーを追加する
            scene.AddLayer(layer);

            // シーンを切り替える
            asd.Engine.ChangeScene(scene);

            {
                // フォントを生成する
                var smallFont = asd.Engine.Graphics.CreateFont("Data/Font/small.aff");

                //TextObject2Dを生成する
                var smallObj = new asd.TextObject2D();

                //描画に使うフォントを設定する
                smallObj.Font = smallFont;

                //描画位置を指定する
                smallObj.Position = new asd.Vector2DF(100, 100);

                //描画する文字列の指定
                smallObj.Text = "文字列サンプル1";

                //レイヤーへの追加
                layer.AddObject(smallObj);
            }

            {
                var largeFont = asd.Engine.Graphics.CreateFont("Data/Font/large.aff");
                var largeObj = new asd.TextObject2D();
                largeObj.Font = largeFont;
                largeObj.Position = new asd.Vector2DF(100, 200);
                largeObj.Text = "文字列サンプル2";
                layer.AddObject(largeObj);
            }

            {
                var edgeFont = asd.Engine.Graphics.CreateFont("Data/Font/edge.aff");
                var edgeObj = new asd.TextObject2D();
                edgeObj.Font = edgeFont;
                edgeObj.Position = new asd.Vector2DF(100, 300);

                //30度回転させて描画する
                edgeObj.Angle = 30;
                edgeObj.Text = "文字列サンプル3";
                layer.AddObject(edgeObj);
            }
            // aceが進行可能かチェックする。
            while (asd.Engine.DoEvents())
            {
                // aceを更新する。
                asd.Engine.Update();
                asd.Engine.TakeScreenshot("TextObject2D.png");
            }

            // aceを終了する。
            asd.Engine.Terminate();
        }
    }
}