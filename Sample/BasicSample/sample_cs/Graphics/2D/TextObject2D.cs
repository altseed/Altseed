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
            // AC-Engineを初期化する。
            ace.Engine.Initialize("TextObject2D", 640, 480, new ace.EngineOption());

            // シーンを生成する
            var scene = new ace.Scene();

            // レイヤーを生成する
            var layer = new ace.Layer2D();

            // シーンにレイヤーを追加する
            scene.AddLayer(layer);

            // シーンを切り替える
            ace.Engine.ChangeScene(scene);

            {
                // フォントを生成する
                var smallFont = ace.Engine.Graphics.CreateFont("Data/Font/small.aff");

                //TextObject2Dを生成する
                var smallObj = new ace.TextObject2D();

                //描画に使うフォントを設定する
                smallObj.Font = smallFont;

                //描画位置を指定する
                smallObj.Position = new ace.Vector2DF(100, 100);

                //描画する文字列の指定
                smallObj.Text = "文字列サンプル1";

                //レイヤーへの追加
                layer.AddObject(smallObj);
            }

            {
                var largeFont = ace.Engine.Graphics.CreateFont("Data/Font/large.aff");
                var largeObj = new ace.TextObject2D();
                largeObj.Font = largeFont;
                largeObj.Position = new ace.Vector2DF(100, 200);
                largeObj.Text = "文字列サンプル2";
                layer.AddObject(largeObj);
            }

            {
                var edgeFont = ace.Engine.Graphics.CreateFont("Data/Font/edge.aff");
                var edgeObj = new ace.TextObject2D();
                edgeObj.Font = edgeFont;
                edgeObj.Position = new ace.Vector2DF(100, 300);

                //30度回転させて描画する
                edgeObj.Angle = 30;
                edgeObj.Text = "文字列サンプル3";
                layer.AddObject(edgeObj);
            }
            // AC-Engineのウインドウが閉じられていないか確認する。
            while (ace.Engine.DoEvents())
            {
                // AC-Engineを更新する。
                ace.Engine.Update();
                ace.Engine.TakeScreenshot("TextObject2D.png");
            }

            // AC-Engineの終了処理をする。
            ace.Engine.Terminate();
        }
    }
}