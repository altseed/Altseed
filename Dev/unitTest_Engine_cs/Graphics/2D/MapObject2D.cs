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
            var scene = new ace.Scene();
            var layer = new ace.Layer2D();
            var obj = new ace.MapObject2D();

            Assert.AreNotEqual<ace.Scene>(scene, null);
            Assert.AreNotEqual<ace.Layer>(layer, null);
            Assert.AreNotEqual<ace.MapObject2D>(obj, null);

            ace.Engine.ChangeScene(scene);
            scene.AddLayer(layer);
            layer.AddObject(obj);

            var g = ace.Engine.Graphics;
            var texture = g.CreateTexture2D("Data/Map/MapTest.png");

            var chip1 = g.CreateChip2D();
            chip1.Texture = texture;
            chip1.Src = new ace.RectF(0, 0, 256, 256);
            obj.AddChip(chip1);

            var chip2 = g.CreateChip2D();
            chip2.Texture = texture;
            chip2.Src = new ace.RectF(300, 0, 256, 256);
            obj.AddChip(chip2);

            obj.Position = new ace.Vector2DF(0, 0);
        }

        protected override void OnUpdating()
        {

        }
    }
}