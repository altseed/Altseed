using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs
{
    class SceneAndLayer_Basic : ISample
    {
        // シーンとレイヤーを手動で生成する。
        public void Run()
        {
            // Altseedを初期化する
            asd.Engine.Initialize("SceneAndLayer_Basic", 640, 480, new asd.EngineOption());

            var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Picture1.png");

            // シーンのコンストラクタを呼び出す。
            var scene = new asd.Scene();

            // レイヤーのコンストラクタを呼び出す。
            var layer = new asd.Layer2D();

            // オブジェクトのコンストラクタを呼び出す。
            var obj = new asd.TextureObject2D();

            // オブジェクトの位置とテクスチャを設定。
            obj.Position = new asd.Vector2DF(50, 50);
            obj.Texture = texture;
            
            // 描画するシーンを指定する。
            asd.Engine.ChangeScene(scene);

            // 描画するレイヤーをシーンに追加する。
            scene.AddLayer(layer);

            // 描画するオブジェクトをレイヤーに追加する。
            layer.AddObject(obj);

            while (asd.Engine.DoEvents())
            {
                asd.Engine.Update();
            }

            asd.Engine.Terminate();
        }
    }
}
