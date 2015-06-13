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
    class TextureObject2D : ISample
    {
        public void Run()
        {
            // aceを初期化する
            asd.Engine.Initialize("TextureObject2D", 640, 480, new asd.EngineOption());

            // シーンを生成する
            var scene = new asd.Scene();

            // レイヤーを生成する
            var layer = new asd.Layer2D();

            // シーンにレイヤーを追加する
            scene.AddLayer(layer);

            // シーンを切り替える
            asd.Engine.ChangeScene(scene);

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
                layer.AddObject(obj0);
            }

            {
                var tex1 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample2.png");

                var obj1 = new asd.TextureObject2D();

                obj1.Texture= tex1;

                obj1.Position=new asd.Vector2DF(500, 400);

                //画像を上下に反転する
                obj1.TurnUL = true;

                layer.AddObject(obj1);
            }

            var obj2 = new asd.TextureObject2D();
            {

                var tex2 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Character.png");

                obj2.Texture = tex2;

                obj2.Position = new asd.Vector2DF(500, 50);

                layer.AddObject(obj2);
            }


            var obj3 = new asd.TextureObject2D();
            {

                var tex3 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");

                //画像(サイズ 512 x 512)の中心(256 x 256 の地点)を描画の起点とする
                obj3.CenterPosition = new asd.Vector2DF(256, 256);
                obj3.Texture = tex3;

                obj3.Position=new asd.Vector2DF(320, 240);

                //画像を中心を軸に45度回転する
                obj3.Angle = 45;

                //画像をX,Y方向に0.4倍に縮小する
                obj3.Scale = new asd.Vector2DF(0.4f, 0.4f);

                layer.AddObject(obj3);
            }

            {
                var obj4 = new asd.TextureObject2D();

                var tex4 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Character.png");

                obj4.Texture = tex4;

                obj4.Position = new asd.Vector2DF(50, 20);

                layer.AddObject(obj4);
            }


            int count = 0;
            int index = 0;
            // aceが進行可能かチェックする。
            while (asd.Engine.DoEvents())
            {

                int xi = index % 3;
                int yi = (index / 3) % 4;

                //テクスチャの切り出し範囲を指定する。
                obj2.Src = new asd.RectF(32 * xi, 32 * yi, 32, 32);

                if (count % 5 == 0)
                {
                    index = (++index) % 12;
                }
                ++count;

                obj3.Angle += 0.5f;

                // aceを更新する。
                asd.Engine.Update();
            }

            // aceを終了する。
            asd.Engine.Terminate();
        }
    }
}
