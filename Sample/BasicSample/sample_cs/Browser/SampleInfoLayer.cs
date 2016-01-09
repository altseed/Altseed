using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace sample_cs
{
    class SampleInfoLayer : Layer2D
    {
        private static readonly float PanelHeight = 65;
        private static readonly float ScrollBerWidth = 16;

        private TextObject2D title { get; set; }
        private TextObject2D description { get; set; }
        private GeometryObject2D scrollBar { get; set; }
        private float totalHeight { get; set; }
        private float cameraSize { get; set; }

        public SampleInfoLayer(float scrollBarHeight, float totalHeight, float cameraSize)
        {
            scrollBar = new GeometryObject2D()
            {
                Shape = new RectangleShape
                {
                    DrawingArea = new RectF(0, 0, ScrollBerWidth, scrollBarHeight)
                },
                Color = new Color(64, 64, 64, 255),
                Position = new Vector2DF(640 - ScrollBerWidth - SampleBrowser.Margin, 20 + SampleBrowser.Margin),
            };
            this.totalHeight = totalHeight;
            this.cameraSize = cameraSize;
            AddObject(scrollBar);
        }

        protected override void OnStart()
        {
            Name = "InfoLayer";

            var panel = new GeometryObject2D()
            {
                Shape = new RectangleShape
                {
                    DrawingArea = new RectF(0, 0, 640, PanelHeight),
                },
                Color = new Color(16, 16, 16, 255),
                Position = new Vector2DF(0, 480 - PanelHeight),
            };

            var font = Engine.Graphics.CreateDynamicFont("", 12, new Color(255, 255, 255, 255), 1, new Color(0, 0, 0, 255));
            title = new TextObject2D()
            {
                Font = font,
                Text = "",
                Color = new Color(255, 255, 0),
                Position = new Vector2DF(2, 2),
                DrawingPriority = 1,
            };

            description = new TextObject2D()
            {
                Font = font,
                Text = "",
                Color = new Color(255, 255, 255),
                Position = new Vector2DF(6, 22),
                DrawingPriority = 1,
            };

            panel.AddChild(title, ChildManagementMode.Nothing, ChildTransformingMode.Position);
            panel.AddChild(description, ChildManagementMode.Nothing, ChildTransformingMode.Position);

            AddObject(panel);
            AddObject(title);
            AddObject(description);
        }

        public void Show(ISample sample)
        {
            if(sample == null)
            {
                title.Text = "";
                description.Text = "";
            }
            else
            {
                title.Text = sample.GetType().Name;
                description.Text = sample.Description;
            }
        }

        public void MoveScrollBar(float pos)
        {
            float yOffset = pos / totalHeight * (480 - 20 - PanelHeight - SampleBrowser.Margin * 2);
            scrollBar.Position = new Vector2DF(640 - ScrollBerWidth - SampleBrowser.Margin, 20 + SampleBrowser.Margin + yOffset);
        }
    }
}
