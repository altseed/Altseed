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
        {
            // aceを初期化する。
            ace.Engine.Initialize("CameraObject2D", 640, 480, new ace.EngineOption());

            // シーンを生成する。
            var scene = new ace.Scene();

            // レイヤーを生成する。
            var layer = new ace.Layer2D();

            // シーンにレイヤーを追加する。
            scene.AddLayer(layer);

            // シーンを切り替える。
            ace.Engine.ChangeScene(scene);


            {
                // テクスチャを生成する。
                var tex0 = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");
                var obj0 = new ace.TextureObject2D();
                obj0.Texture = tex0;
                obj0.CenterPosition = new ace.Vector2DF(256, 256);
                obj0.Position = new ace.Vector2DF(320, 240);
                obj0.Scale = new ace.Vector2DF(0.5f, 0.5f);

                layer.AddObject(obj0);
            }

            //画面全体を写すカメラ。(オブジェクトをそのまま描画)
            {
                var camera1 = new ace.CameraObject2D();
                camera1.Src = new ace.RectI(0, 0, 640, 480);
                camera1.Dst = new ace.RectI(0, 0, 640, 480);
                layer.AddObject(camera1);
            }

            //マウスポインタの周辺を拡大して表示するカメラ。
            var camera2 = new ace.CameraObject2D();
            layer.AddObject(camera2);

            //フレーム用テクスチャ画像
            var frame = new ace.TextureObject2D();
            {
                var tex = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Frame.png");
                frame.Texture = tex;
                frame.CenterPosition = new ace.Vector2DF(55.0f, 55.0f);

                layer.AddObject(frame);
            }

            // aceが進行可能かチェックする。
            while (ace.Engine.DoEvents())
            {
                //マウスポインタの位置を取得する。
                var pos = ace.Engine.Mouse.Position;

                //拡大用カメラの描画元を指定する。
                camera2.Src = new ace.RectI((int)(pos.X) - 25, (int)(pos.Y) - 25, 50, 50);

                //ポインタを中心に100x100の拡大画像を表示する。
                camera2.Dst = new ace.RectI((int)(pos.X) - 50, (int)(pos.Y) - 50, 100, 100);

                //フレーム画像の描画中心をマウスポインタの位置に合わせる。
                frame.Position = pos;

                // aceを更新する。
                ace.Engine.Update();
            }

            // aceを終了する。
            ace.Engine.Terminate();
        }
    }
}
