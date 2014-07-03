using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
/*
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
            var scene = new ace.Scene();
            var layer = new ace.Layer2D();
            var obj = new ace.TextObject2D();

            Assert.AreNotEqual<ace.Scene>(scene, null);
            Assert.AreNotEqual<ace.Layer>(layer, null);
            Assert.AreNotEqual<ace.TextObject2D>(obj, null);

            ace.Engine.ChangeScene(scene);
            scene.AddLayer(layer);
            layer.AddObject(obj);

            var g = ace.Engine.Graphics;

            var font = g.CreateFont("Data/Text/Nac0703.aff");

            obj.Font = font;
            obj.Text = "Font";

            //object->SetCenterPosition(Vector2DF(128, 128));
            obj.Position = new ace.Vector2DF(0, 0);
        }

        protected override void OnUpdating()
        {

        }
    }
}
*/