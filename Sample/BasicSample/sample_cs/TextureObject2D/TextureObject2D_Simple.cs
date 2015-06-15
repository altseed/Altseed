using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// <summary>
/// テクスチャを表示するサンプル。
/// </summary>
class TextureObject2D_Simple : ISample
{
    public void Run()
    {
        // Altseedを初期化する
        asd.Engine.Initialize("TextureObject2D_Simple", 640, 480, new asd.EngineOption());

        {
            // テクスチャを生成する
            var tex0 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample2.png");

            // TextureObject2Dを生成する
            var obj0 = new asd.TextureObject2D();

            // 描画に使うテクスチャを設定する
            obj0.Texture = tex0;

            // 描画位置を指定する
            obj0.Position = new asd.Vector2DF(100, 400);

            // レイヤーへ追加する
            asd.Engine.AddObject2D(obj0);
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