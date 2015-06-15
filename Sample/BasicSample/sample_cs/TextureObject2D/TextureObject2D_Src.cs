using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// <summary>
/// テクスチャを一部切り出して表示するサンプル。
/// </summary>
class TextureObject2D_Src : ISample
{
    public void Run()
    {
        // Altseedを初期化する
        asd.Engine.Initialize("TextureObject2D_Src", 640, 480, new asd.EngineOption());

        // 実際に切り出すテクスチャを描画するオブジェクトを設定する。
        var obj2 = new asd.TextureObject2D();
        {

            var tex2 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Character.png");

            obj2.Texture = tex2;

            obj2.Position = new asd.Vector2DF(500, 50);

            asd.Engine.AddObject2D(obj2);
        }

        // 切りだす前のテクスチャ描画用オブジェクトを設定する。
        {
            var obj4 = new asd.TextureObject2D();

            var tex4 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Character.png");

            obj4.Texture = tex4;

            obj4.Position = new asd.Vector2DF(50, 20);

            asd.Engine.AddObject2D(obj4);
        }


        int count = 0;
        int index = 0;
        // Altseedが進行可能かチェックする。
        while (asd.Engine.DoEvents())
        {

            int xi = index % 3;
            int yi = (index / 3) % 4;

            // テクスチャの切り出し範囲を指定する。
            obj2.Src = new asd.RectF(32 * xi, 32 * yi, 32, 32);

            if (count % 5 == 0)
            {
                index = (++index) % 12;
            }
            ++count;

            // Altseedを更新する。
            asd.Engine.Update();
        }

        // Altseedを終了する。
        asd.Engine.Terminate();
    }
}
