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
        // aceを初期化する
        asd.Engine.Initialize("TextureObject2D", 640, 480, new asd.EngineOption());

        var obj3 = new asd.TextureObject2D();
        {

            var tex3 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");

            //画像(サイズ 512 x 512)の中心(256 x 256 の地点)を描画の起点とする
            obj3.CenterPosition = new asd.Vector2DF(256, 256);
            obj3.Texture = tex3;

            obj3.Position = new asd.Vector2DF(320, 240);

            //画像を中心を軸に45度回転する
            obj3.Angle = 45;

            //画像をX,Y方向に0.4倍に縮小する
            obj3.Scale = new asd.Vector2DF(0.4f, 0.4f);

            asd.Engine.AddObject2D(obj3);
        }

        // aceが進行可能かチェックする。
        while (asd.Engine.DoEvents())
        {
            //オブジェクトを0.5度回転させる。
            obj3.Angle += 0.5f;

            // aceを更新する。
            asd.Engine.Update();
        }

        // aceを終了する。
        asd.Engine.Terminate();
    }
}