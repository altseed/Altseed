using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.ObjectSystem
{
    class Scene1 : ace.Scene
    {
        private ace.TextureObject2D obj;
        protected override void OnUpdated()
        {
            obj.Angle += 2.0f;

            //マウスが左クリックされた時にScene2へ遷移する。
            if (ace.Engine.Mouse.LeftButton.ButtonState == ace.MouseButtonState.Push)
            {
                var scene2 = new Scene2();
                ace.Engine.ChangeScene(scene2);
            }
        }

        public Scene1()
        {
            var layer = new ace.Layer2D();

            AddLayer(layer);

            obj = new ace.TextureObject2D();
            obj.Texture = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");
            obj.CenterPosition = new ace.Vector2DF(256, 256);
            obj.Position = new ace.Vector2DF(320, 240);

            layer.AddObject(obj);
        }
    };

    class Scene2 : ace.Scene
    {
        private ace.TextureObject2D obj;
        protected override void OnUpdated()
        {
            obj.Angle -= 2.0f;

            //マウスが左クリックされた時にフェード付きでScene1へ遷移する。
            if (ace.Engine.Mouse.LeftButton.ButtonState == ace.MouseButtonState.Push)
            {
                var scene1 = new Scene1();
                ace.Engine.ChangeSceneWithTransition(scene1, new ace.TransitionFade(1.0f, 1.0f));
            }
        }

        public Scene2()
        {
            var layer = new ace.Layer2D();

            AddLayer(layer);

            obj = new ace.TextureObject2D();
            obj.Texture = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Block1.png");
            obj.Position = new ace.Vector2DF(100, 100);

            layer.AddObject(obj);
        }
    };

    public class Transition : ISample
    {
        public void Run()
        {
            // aceを初期化する
            ace.Engine.Initialize("Transition", 640, 480, new ace.EngineOption());

            // シーンを生成する
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
