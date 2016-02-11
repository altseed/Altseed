using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs
{
    class PostEffect_LightBloom : ISample
    {
        public string Description
        {
            get { return ""; }
        }
		public string Title
		{
			get { return ""; }
		}

        // レイヤーにライトブルームのポストエフェクトを適用する。
        public void Run()
        {
            // Altseedを初期化する。
            asd.Engine.Initialize("PostEffect_LightBloom", 640, 480, new asd.EngineOption());

            var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Picture1.png");

            // シーンクラスのインスタンスを生成する。
            var scene = new asd.Scene();

            // レイヤーのコンストラクタを呼び出す。
            var layer = new asd.Layer2D();

            // レイヤークラスのインスタンスを生成する。
            var obj = new asd.TextureObject2D();

            // 画像描画オブジェクトのインスタンスを生成する。
            obj.Position = new asd.Vector2DF(50, 50);
            obj.Texture = texture;

            // シーンを変更し、そのシーンにレイヤーを追加し、そのレイヤーにオブジェクトを追加する。
            asd.Engine.ChangeScene(scene);
            scene.AddLayer(layer);
            layer.AddObject(obj);

            // ライトブルームクラスのインスタンスを生成する。
            var posteffect = new asd.PostEffectLightBloom();

            // ライトブルームのぼかしの強さを設定する。
            posteffect.Intensity = 10.0f;

            // ライトブルームの露光の強さを設定する。
            posteffect.Exposure = 1.0f;

            // ライトブルームで光らせる明るさのしきい値を設定する。
            posteffect.Threshold = 0.3f;

            // レイヤーにライトブルームのポストエフェクトを適用。
            layer.AddPostEffect(posteffect);
            
            // Altseedのウインドウが閉じられていないか確認する。
            while (asd.Engine.DoEvents())
            {
                // Altseedを更新する。
                asd.Engine.Update();
            }
            
            // Altseedを終了する。
            asd.Engine.Terminate();
        }
    }
}
