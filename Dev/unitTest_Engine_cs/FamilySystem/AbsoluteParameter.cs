using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.FamilySystem
{
    class AbsoluteParameter : EngineTest
    {
        public AbsoluteParameter() : base(60)
        {
        }

	    private TextureObject2D child;
	    private TextureObject2D child2;

        protected override void OnStart()
        {
            var parent = new TextureObject2D()
            {
                Position = new Vector2DF(40, 40),
                Color = new Color(128, 255, 255, 255),
                DrawingPriority = 2,
                IsUpdated = false,
                IsDrawn = false,
            };
            child = new TextureObject2D()
            {
                Position = new Vector2DF(100, 0),
                Color = new Color(255, 255, 128, 255),
                DrawingPriority = 1,
                IsUpdated = true,
                IsDrawn = true,
            };
            child2 = new TextureObject2D()
            {
                Position = new Vector2DF(0, 100),
                Color = new Color(255, 255, 128, 255),
                DrawingPriority = 1,
                IsUpdated = true,
                IsDrawn = true,
            };
            parent.AddDrawnChild(child,
                ChildManagementMode.IsDrawn | ChildManagementMode.IsUpdated,
                ChildTransformingMode.All,
                ChildDrawingMode.Color | ChildDrawingMode.DrawingPriority);
            parent.AddDrawnChild(child2,
                ChildManagementMode.Nothing,
                ChildTransformingMode.All,
                ChildDrawingMode.Nothing);

            Engine.AddObject2D(parent);
            Engine.AddObject2D(child);
            Engine.AddObject2D(child2);
        }

	    protected override void OnUpdated()
	    {
		    if (Time == 1)
			{
				Assert.AreEqual(child.GetGlobalPosition(), new Vector2DF(140, 40));
				Assert.AreEqual(child.AbsoluteBeingUpdated, false);
				Assert.AreEqual(child.AbsoluteBeingDrawn, false);
				Assert.AreEqual(child.AbsoluteColor, new Color(128, 255, 128, 255));
				Assert.AreEqual(child.AbsoluteDrawingPriority, 3);

				Assert.AreEqual(child2.GetGlobalPosition(), new Vector2DF(40, 140));
				Assert.AreEqual(child2.AbsoluteBeingUpdated, true);
				Assert.AreEqual(child2.AbsoluteBeingDrawn, true);
				Assert.AreEqual(child2.AbsoluteColor, new Color(255, 255, 128, 255));
				Assert.AreEqual(child2.AbsoluteDrawingPriority, 1);
			}
	    }
    }
}
