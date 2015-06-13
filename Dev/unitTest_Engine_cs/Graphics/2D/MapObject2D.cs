using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics._2D
{
    class MapObject2D : EngineTest
    {
        public MapObject2D()
            : base(20)
        {
        }

        protected override void OnStart()
        {
            var scene = new asd.Scene();
            var layer = new asd.Layer2D();
            var obj = new asd.MapObject2D();

            Assert.AreNotEqual<asd.Scene>(scene, null);
            Assert.AreNotEqual<asd.Layer>(layer, null);
            Assert.AreNotEqual<asd.MapObject2D>(obj, null);

            asd.Engine.ChangeScene(scene);
            scene.AddLayer(layer);
            layer.AddObject(obj);

            var g = asd.Engine.Graphics;
            var texture = g.CreateTexture2D("Data/Map/MapTest.png");

            var chip1 = new asd.Chip2D();
            chip1.Texture = texture;
            chip1.Position = new asd.Vector2DF(0, 0);
            obj.AddChip(chip1);

            var chip2 = new asd.Chip2D();
            chip2.Texture = texture;
            chip2.Position = new asd.Vector2DF(300, 0);
            obj.AddChip(chip2);

            obj.Position = new asd.Vector2DF(0, 0);
        }

        protected override void OnUpdating()
        {

        }
    }
}