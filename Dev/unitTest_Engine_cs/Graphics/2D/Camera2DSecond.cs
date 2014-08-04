using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.ObjectSystem2D
{
    class Camera2DSecond : EngineTest
    {
        private ace.CameraObject2D camera;

        public Camera2DSecond()
            : base(60)
        {
        }

        protected override void OnStart()
        {
            var scene = new ace.Scene();
            var layer = new ace.Layer2D();
            var obj = new ace.TextureObject2D();
            camera = new ace.CameraObject2D();

            obj.Texture = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Cloud1.png");
            camera.Src = new ace.RectI(0+3900, 0, 640+3900, 480);
            camera.Dst = new ace.RectI(0, 0, 640, 480);

            obj.Position = new ace.Vector2DF(4000, 20);

            layer.AddObject(obj);
            layer.AddObject(camera);
            scene.AddLayer(layer);
            ace.Engine.ChangeScene(scene);
        }

        protected override void OnUpdating()
        {

        }
    }
}
