using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics._2D
{
    class TextureObject2D : EngineTest
    {
        public TextureObject2D()
            : base(20)
        {
        }

        protected override void OnStart()
        {
            var scene = new asd.Scene();
            var layer = new asd.Layer2D();
            var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");
            var textureObj1 = new asd.TextureObject2D();
            var textureObj2 = new asd.TextureObject2D();

            layer.AddObject(textureObj1);
            layer.AddObject(textureObj2);
            scene.AddLayer(layer);
            asd.Engine.ChangeScene(scene);

            textureObj1.Texture = texture;
            textureObj1.Src = new asd.RectF(256, 256, 256, 256);
            textureObj1.Position=new asd.Vector2DF(320, 240);

            textureObj2.Texture = texture;
            textureObj2.Src = new asd.RectF(0, 0, 256, 256);
            textureObj2.Position = new asd.Vector2DF(0, 0);
            textureObj2.TurnLR = true;
        }

        protected override void OnUpdating()
        {

        }
    }
}