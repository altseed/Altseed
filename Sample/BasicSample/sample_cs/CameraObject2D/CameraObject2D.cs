using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class CameraObject2D : ISample
{
    public void Run()
    {
        // aceを初期化する。
        asd.Engine.Initialize("CameraObject2D", 640, 480, new asd.EngineOption());


        // テクスチャを生成する。
        {
            var tex0 = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");
            var obj0 = new asd.TextureObject2D();
            obj0.Texture = tex0;
            obj0.CenterPosition = new asd.Vector2DF(256, 256);
            obj0.Position = new asd.Vector2DF(320, 240);
            obj0.Scale = new asd.Vector2DF(0.5f, 0.5f);

            asd.Engine.AddObject2D(obj0);
        }

        //一つ目の画面全体を写すカメラ。(オブジェクトをそのまま描画する。)
        {
            var camera1 = new asd.CameraObject2D();
            camera1.Src = new asd.RectI(0, 0, 640, 480);
            camera1.Dst = new asd.RectI(0, 0, 640, 480);
            asd.Engine.AddObject2D(camera1);
        }

        //二つ目のマウスポインタの周辺を拡大して表示するカメラ。
        var camera2 = new asd.CameraObject2D();
        asd.Engine.AddObject2D(camera2);

        //フレーム用テクスチャ画像を準備する。
        var frame = new asd.TextureObject2D();
        {
            var tex = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Frame.png");
            frame.Texture = tex;
            frame.CenterPosition = new asd.Vector2DF(55.0f, 55.0f);

            asd.Engine.AddObject2D(frame);
        }

        // aceが進行可能かチェックする。
        while (asd.Engine.DoEvents())
        {
            //マウスポインタの位置を取得する。
            var pos = asd.Engine.Mouse.Position;

            //拡大用カメラの描画元を指定する。
            camera2.Src = new asd.RectI((int)(pos.X) - 25, (int)(pos.Y) - 25, 50, 50);

            //ポインタを中心に100x100の拡大画像を表示する。
            camera2.Dst = new asd.RectI((int)(pos.X) - 50, (int)(pos.Y) - 50, 100, 100);

            //フレーム画像の描画中心をマウスポインタの位置に合わせる。
            frame.Position = pos;

            // aceを更新する。
            asd.Engine.Update();
        }

        // aceを終了する。
        asd.Engine.Terminate();
    }
}
