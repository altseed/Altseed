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
    class TextureObject2D_Turn : ISample
    {
        public void Run()
        {
            // aceを初期化する
            asd.Engine.Initialize("TextureObject2D_Turn", 640, 480, new asd.EngineOption());

            {
                var tex1 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample2.png");

                var obj1 = new asd.TextureObject2D();

                obj1.Texture = tex1;

                obj1.Position = new asd.Vector2DF(500, 400);

                //画像を上下に反転する
                obj1.TurnUL = true;

                asd.Engine.AddObject2D(obj1);
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
