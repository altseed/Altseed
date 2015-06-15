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
        // Altseedを初期化する。
        asd.Engine.Initialize("TextureObject2D_Simple", 640, 480, new asd.EngineOption());

        // テクスチャを読み込む。
		var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");

        // TextureObject2Dを生成する
        var obj = new asd.TextureObject2D();

        // 描画に使うテクスチャを設定する
        obj.Texture = texture;

        // 描画位置を指定する
        obj.Position = new asd.Vector2DF(100, 100);

        // オブジェクトのインスタンスをエンジンに追加する。
        asd.Engine.AddObject2D(obj);

        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            // Altseedを更新する。
            asd.Engine.Update();
			Recorder.TakeScreenShot("TextureObject2D_Simple", 5);
        }

        // Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}