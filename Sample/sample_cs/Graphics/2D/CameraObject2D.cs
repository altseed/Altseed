using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.Graphics._2D
{
    public class CameraObject2D : ISample
    {
        public void Run()
        {   // aceを初期化する
            ace.Engine.Initialize("CameraObject2D", 640, 480, new ace.EngineOption());

            // シーンを生成する
            var scene = new ace.Scene();

            // レイヤーを生成する
            var layer = new ace.Layer2D();

            // シーンにレイヤーを追加する
            scene.AddLayer(layer);

            // シーンを切り替える
            ace.Engine.ChangeScene(scene);


            {
                // テクスチャを生成する
                var tex0 = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Sample2.png");

                //TextureObject2Dを生成する
                var obj0 = new ace.TextureObject2D();

                //描画に使うテクスチャを設定する
                obj0.Texture = tex0;

                //描画位置を指定する
                obj0.Position = new ace.Vector2DF(100, 100);

                //レイヤーへ追加する
                layer.AddObject(obj0);
            }

            var camera1 = new ace.CameraObject2D();
            camera1.Src = new ace.RectI(0, 0, 200, 200);
            camera1.Dst = new ace.RectI(0, 0, 200, 200);

            layer.AddObject(camera1);


            // aceが進行可能かチェックする。
            while (ace.Engine.DoEvents())
            {
                // aceを更新する。
                ace.Engine.Update();
            }

            // aceを終了する。
            ace.Engine.Terminate();
        }
    }
}
