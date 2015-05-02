using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics._2D
{

    class AdditionallyDraw : EngineTest
    {
        class CustomObject : ace.TextureObject2D
        {
            public CustomObject()
                : base()
            {

            }

            protected override void OnDrawAdditionally()
            {
                DrawCircleAdditionally(new ace.Vector2DF(320, 800 + 240), 1600, 1570, new ace.Color(255, 255, 255, 255), 32, 0, null, ace.AlphaBlendMode.Add, 0);
            }
        }

        public AdditionallyDraw()
            : base(60)
        {
        }

        protected override void OnStart()
        {
            var scene = new ace.Scene();


            var layer = new ace.Layer2D();

            scene.AddLayer(layer);

            var obj = new CustomObject();
            layer.AddObject(obj);

            ace.Engine.ChangeScene(scene);
        }

        protected override void OnUpdating()
        {

        }
    }
}
