using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace unitTest_Engine_cs.ObjectSystem2D
{
    class ComponentError0408 : EngineTest
    {
        class MyComponent : Object2DComponent
        {
            private Action<float> setter;
            private float time;

            public MyComponent(Action<float> setter)
            {
                this.setter = setter;
            }

            protected override void OnUpdate()
            {
                time += Engine.DeltaTime * 10;
                setter(time);

                if (time > 100) Owner.Vanish();
            }
        }

        class MyObject : TextureObject2D
        {
            TextureObject2D child;
            TextObject2D c2;
            public MyObject()
            {
                Position = new Vector2DF(320, 0);
                child = new TextureObject2D()
                {
                    Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath),
                };
                AddChild(child, ChildMode.Position);

                c2 = new ace.TextObject2D();
                var font = ace.Engine.Graphics.CreateFont("Data/Font/Nac0812.aff");
                c2.Font = font;
                c2.Text = "DependencyPropertyは\n依存関係プロパティのこと";
                AddChild(c2, ChildMode.Position);

                AddComponent(new MyComponent(v => Position = new Vector2DF(Position.X, v)), "Hoge");
            }

            protected override void OnStart()
            {
                Layer.AddObject(child);
                Layer.AddObject(c2);
            }
        }

        public ComponentError0408()
            : base(60000000)
        {
        }

        Layer2D layer = null;

        protected override void OnStart()
        {
            var scene = new Scene();
            layer = new Layer2D();
            var obj = new MyObject();

            Engine.ChangeScene(scene);
            scene.AddLayer(layer);
            layer.AddObject(obj);
        }

        int time = 0;

        protected override void OnUpdating()
        {
            if (time % 30 == 0)
            {
                var obj = new MyObject();
                layer.AddObject(obj);
            }
            time++;
            base.OnUpdating();
        }
    }
}
