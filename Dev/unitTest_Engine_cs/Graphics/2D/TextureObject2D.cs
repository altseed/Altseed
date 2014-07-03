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
            var scene = new ace.Scene();
            var layer = new ace.Layer2D();
            var texture = ace.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");
            var textureObj1 = new ace.TextureObject2D();
            var textureObj2 = new ace.TextureObject2D();

            layer.AddObject(textureObj1);
            layer.AddObject(textureObj2);
            scene.AddLayer(layer);
            ace.Engine.ChangeScene(scene);

            textureObj1.Texture = texture;
            textureObj1.Src = new ace.RectF(256, 256, 256, 256);
            textureObj1.Position=new ace.Vector2DF(320, 240);

            textureObj2.Texture = texture;
            textureObj2.Src = new ace.RectF(0, 0, 256, 256);
            textureObj2.Position = new ace.Vector2DF(0, 0);
            textureObj2.TurnLR = true;
        }

        void OnUpdating()
        {

        }
    }
}