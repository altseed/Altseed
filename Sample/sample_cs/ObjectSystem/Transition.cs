using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.ObjectSystem
{
    /// <summary>
    /// Sample1.pngを右回転させながら表示するクラス。
    /// </summary>
    class Scene1 : ace.Scene
    {
        private ace.TextureObject2D obj;

        public Scene1()
        {
            //レイヤーを追加する。
            var layer = new ace.Layer2D();
            AddLayer(layer);

            //Sample1.pngを描画するテクスチャオブジェクトを設定する。
            obj = new ace.TextureObject2D();
            obj.Texture = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");
            obj.CenterPosition = new ace.Vector2DF(256, 256);
            obj.Position = new ace.Vector2DF(320, 240);
            layer.AddObject(obj);
        }

        protected override void OnUpdated()
        {
            //Sample1.pngを右方向に2度回転する。
            obj.Angle += 2.0f;

            //マウスが左クリックされた時にScene2へ遷移する。
            if (ace.Engine.Mouse.LeftButton.ButtonState == ace.MouseButtonState.Push)
            {
                var scene2 = new Scene2();
                ace.Engine.ChangeScene(scene2);
            }
        }
    };

    /// <summary>
    /// Block1.pngを左回転させながら表示するクラス。
    /// </summary>
    class Scene2 : ace.Scene
    {
        private ace.TextureObject2D obj;

        public Scene2()
        {
            //レイヤーを追加する。
            var layer = new ace.Layer2D();
            AddLayer(layer);

            //Block1.pngを描画するテクスチャオブジェクトを設定する。
            obj = new ace.TextureObject2D();
            obj.Texture = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Block1.png");
            obj.Position = new ace.Vector2DF(100, 100);
            layer.AddObject(obj);
        }

        protected override void OnUpdated()
        {
            //Block1.pngを左方向に2度回転する。
            obj.Angle -= 2.0f;

            //マウスが左クリックされた時にフェード付きでScene1へ遷移する。
            //（フェードアウトとフェードインはそれぞれ1秒間かけて行う。）
            if (ace.Engine.Mouse.LeftButton.ButtonState == ace.MouseButtonState.Push)
            {
                var scene1 = new Scene1();
                ace.Engine.ChangeSceneWithTransition(scene1, new ace.TransitionFade(1.0f, 1.0f));
            }
        }
    };

    public class Transition : ISample
    {
        public void Run()
        {
            // aceを初期化する
            ace.Engine.Initialize("Transition", 640, 480, new ace.EngineOption());

            // シーンをScene1に設定する。
            var scene = new Scene1();
            ace.Engine.ChangeScene(scene);

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
