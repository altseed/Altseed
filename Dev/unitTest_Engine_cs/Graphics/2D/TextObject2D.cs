using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics._2D
{
    class TextObject2D : EngineTest
    {
        public TextObject2D()
            : base(20)
        {
        }

        protected override void OnStart()
        {
            var scene = new asd.Scene();
            var layer = new asd.Layer2D();
            var obj = new asd.TextObject2D();

            Assert.AreNotEqual<asd.Scene>(scene, null);
            Assert.AreNotEqual<asd.Layer>(layer, null);
            Assert.AreNotEqual<asd.TextObject2D>(obj, null);

            asd.Engine.ChangeScene(scene);
            scene.AddLayer(layer);
            layer.AddObject(obj);

			var font = asd.Engine.Graphics.CreateFont("Data/Font/Nac0812.aff");

            obj.Font = font;
			obj.Text = "DependencyPropertyは\n依存関係プロパティのこと";

            //object->SetCenterPosition(Vector2DF(128, 128));
            obj.Position = new asd.Vector2DF(0, 0);
        }

        protected override void OnUpdating()
        {

        }
    }
}
