using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// <summary>
/// テクスチャを上下反転して描画するサンプル。
/// </summary>
class TextureObject2D_Turn : ISample
{
    public void Run()
    {
        // Altseedを初期化する
        asd.Engine.Initialize("TextureObject2D_Turn", 640, 480, new asd.EngineOption());

        {
            var tex1 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample2.png");

            var obj1 = new asd.TextureObject2D();

            obj1.Texture = tex1;

            obj1.Position = new asd.Vector2DF(500, 400);

            // 画像を上下に反転する
            obj1.TurnUL = true;

            asd.Engine.AddObject2D(obj1);
        }

        // Altseedが進行可能かチェックする。
        while (asd.Engine.DoEvents())
        {
            // Altseedを更新する。
            asd.Engine.Update();
        }

        // Altseedを終了する。
        asd.Engine.Terminate();
    }
}