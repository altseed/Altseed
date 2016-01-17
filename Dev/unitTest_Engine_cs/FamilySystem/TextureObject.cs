using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.FamilySystem
{
    class TextureObject : EngineTest
    {
        public TextureObject() : base(60)
        {
        }

        protected override void OnStart()
        {
            var texture = Engine.Graphics.CreateTexture2D(CloudTexturePath);

            var parent = new TextureObject2D()
            {
                Texture = texture,
                Position = new Vector2DF(20, 20),
                Color = new Color(255, 128, 128, 255),
                DrawingPriority = 2,
            };
            var child1 = new TextureObject2D()
            {
                Texture = texture,
                Position = new Vector2DF(0, 100),
                Color = new Color(128, 128, 255, 255),
                DrawingPriority = 1,
            };
            var child2 = new TextureObject2D()
            {
                Texture = texture,
                Position = new Vector2DF(100, 0),
                Color = new Color(128, 128, 255, 255),
                DrawingPriority = 1,
            };

            parent.AddDrawnChild(child1,
                ChildManagementMode.Nothing,
                ChildTransformingMode.All,
                ChildDrawingMode.Color | ChildDrawingMode.DrawingPriority);

            parent.AddDrawnChild(child2,
                ChildManagementMode.Nothing,
                ChildTransformingMode.All,
                ChildDrawingMode.Nothing);

            Engine.AddObject2D(parent);
            Engine.AddObject2D(child1);
            Engine.AddObject2D(child2);
        }
    }
}
