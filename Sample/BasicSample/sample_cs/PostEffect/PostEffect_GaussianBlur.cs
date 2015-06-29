using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs
{
    class PostEffect_GaussianBlur : ISample
    {
        /// <summary>
        /// レイヤーにガウスぼかしのポストエフェクトを適用する。
        /// </summary>
        public void Run()
        {
            // Altseedを初期化する
            asd.Engine.Initialize("PostEffect_GaussianBlur", 640, 480, new asd.EngineOption());

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

            // シーンを変更し、そのシーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
            asd.Engine.ChangeScene(scene);
            scene.AddLayer(layer);
            layer.AddObject(obj);

            // レイヤーにガウスぼかしのポストエフェクトを適用。
            layer.AddPostEffect(new asd.PostEffectGaussianBlur());

            while (asd.Engine.DoEvents())
            {
                asd.Engine.Update();
            }

            asd.Engine.Terminate();
        }
    }
}
