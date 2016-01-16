using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D
{
    class DisposeOnVanish : EngineTest
    {
        class MessageLayer : Layer2D
        {
            protected override void OnDispose()
            {
                Console.WriteLine("Layer2D({0}):Dispose", Name);
            }
        }

        class MessageObject : TextureObject2D
        {
            private string name { get; set; }

            public MessageObject(string name)
            {
                this.name = name;
            }

            protected override void OnDispose()
            {
                Console.WriteLine("Object2D({0}):Dispose", name);
            }
        }

        int time = 0;
        Layer2D layer1;
        TextureObject2D obj1;

        public DisposeOnVanish() : base(120)
        {
        }

        protected override void OnStart()
        {
            var scene = new Scene();
            layer1 = new MessageLayer() { Name = "Layer1" };
            var layer2 = new MessageLayer() { Name = "Layer2" };
            obj1 = new MessageObject("Object1");
            var obj2 = new MessageObject("Object2");

            Engine.ChangeScene(scene);
            scene.AddLayer(layer1);
            scene.AddLayer(layer2);
            layer1.AddObject(obj1);
            layer1.AddObject(obj2);
        }

        protected override void OnUpdated()
        {
            time++;
            if(time == 10)
            {
                Console.WriteLine("Vanish Object1");
                obj1.Dispose();
            }
            if(time == 60)
            {
                Console.WriteLine("Vanish Layer1");
                layer1.Dispose();
            }
        }

        protected override void OnFinish()
        {
            Console.WriteLine("Terminate");
        }
    }
}
