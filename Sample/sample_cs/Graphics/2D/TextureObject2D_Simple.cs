using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.Graphics._2D
{
    /// <summary>
    /// テクスチャを表示する。
    /// </summary>
    class TextureObject2D_Simple : ISample
    {
        public void Run()
        {
            // aceを初期化する
            asd.Engine.Initialize("TextureObject2D_Simple", 640, 480, new asd.EngineOption());

            {
                // テクスチャを生成する
                var tex0 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample2.png");

                //TextureObject2Dを生成する
                var obj0 = new asd.TextureObject2D();

                //描画に使うテクスチャを設定する
                obj0.Texture = tex0;

                //描画位置を指定する
                obj0.Position = new asd.Vector2DF(100, 400);

                //レイヤーへ追加する
                asd.Engine.AddObject2D(obj0);
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
