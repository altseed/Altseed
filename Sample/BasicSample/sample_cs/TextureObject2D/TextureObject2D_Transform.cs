using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// <summary>
/// テクスチャを変形させて表示するサンプル。
/// </summary>
class TextureObject2D_Transform : ISample
{
    public void Run()
    {
        // Altseedを初期化する。
        asd.Engine.Initialize("TextureObject2D_Transform", 640, 480, new asd.EngineOption());

        var obj = new asd.TextureObject2D();
        {

            var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");

            // 画像(サイズ 512 x 512)の中心(256 x 256 の地点)を描画の起点とする
            obj.CenterPosition = new asd.Vector2DF(256, 256);
            obj.Texture = texture;

            obj.Position = new asd.Vector2DF(320, 240);

            // 画像を中心を軸に45度回転する
            obj.Angle = 45;

            // 画像をX,Y方向に0.4倍に縮小する
            obj.Scale = new asd.Vector2DF(0.4f, 0.4f);

            asd.Engine.AddObject2D(obj);
        }

        // Altseedのウインドウが閉じられていないか確認する。
        while (asd.Engine.DoEvents())
        {
            // オブジェクトを0.5度回転させる。
            obj.Angle += 0.5f;

            // Altseedを更新する。
            asd.Engine.Update();
        }

        // Altseedの終了処理をする。
        asd.Engine.Terminate();
    }
}