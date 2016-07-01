using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class SampleInfoLayer : asd.Layer2D
{
    private static readonly float PanelHeight = 65;
    private static readonly float ScrollBerWidth = 16;

    private asd.TextObject2D title { get; set; }
    private asd.TextObject2D className { get; set; }
    private asd.TextObject2D description { get; set; }
    private asd.GeometryObject2D scrollBar { get; set; }
    private float totalHeight { get; set; }
    private float cameraSize { get; set; }

    public SampleInfoLayer(float scrollBarHeight, float totalHeight, float cameraSize)
    {
        scrollBar = new asd.GeometryObject2D()
        {
            Shape = new asd.RectangleShape
            {
                DrawingArea = new asd.RectF(0, 0, ScrollBerWidth, scrollBarHeight)
            },
            Color = new asd.Color(64, 64, 64, 255),
            Position = new asd.Vector2DF(640 - ScrollBerWidth - SampleBrowser.Margin, 20 + SampleBrowser.Margin),
        };
        this.totalHeight = totalHeight;
        this.cameraSize = cameraSize;
        AddObject(scrollBar);
    }

    protected override void OnAdded()
    {
        Name = "InfoLayer";

        var panel = new asd.GeometryObject2D()
        {
            Shape = new asd.RectangleShape
            {
                DrawingArea = new asd.RectF(0, 0, 640, PanelHeight),
            },
            Color = new asd.Color(16, 16, 16, 255),
            Position = new asd.Vector2DF(0, 480 - PanelHeight),
        };

        var font = asd.Engine.Graphics.CreateDynamicFont("", 12, new asd.Color(255, 255, 255, 255), 1, new asd.Color(0, 0, 0, 255));
        title = new asd.TextObject2D()
        {
            Font = font,
            Text = "",
            Color = new asd.Color(255, 255, 0),
            Position = new asd.Vector2DF(2, 2),
            DrawingPriority = 1,
        };

        className = new asd.TextObject2D()
        {
            Font = font,
            Text = "",
            Color = new asd.Color(128, 255, 225),
            Position = new asd.Vector2DF(2, 2),
            DrawingPriority = 1,
        };

        description = new asd.TextObject2D()
        {
            Font = font,
            Text = "",
            Color = new asd.Color(255, 255, 255),
            Position = new asd.Vector2DF(6, 22),
            DrawingPriority = 1,
        };

        panel.AddChild(title, asd.ChildManagementMode.Nothing, asd.ChildTransformingMode.Position);
        panel.AddChild(description, asd.ChildManagementMode.Nothing, asd.ChildTransformingMode.Position);
        panel.AddChild(className, asd.ChildManagementMode.RegistrationToLayer, asd.ChildTransformingMode.Position);

        AddObject(panel);
        AddObject(title);
        AddObject(description);
    }

    public void Show(ISample sample)
    {
        if (sample == null)
        {
            title.Text = "";
            description.Text = "";
            className.Text = "";
        }
        else
        {
            title.Text = sample.Title;
            className.Text = "(" + sample.GetType().Name + ")";
            description.Text = sample.Description;
            className.Position = new asd.Vector2DF(title.Font.CalcTextureSize(title.Text, asd.WritingDirection.Horizontal).X + 8, 2);
        }
    }

    public void MoveScrollBar(float pos)
    {
        float yOffset = pos / totalHeight * (480 - 20 - PanelHeight - SampleBrowser.Margin * 2);
        scrollBar.Position = new asd.Vector2DF(640 - ScrollBerWidth - SampleBrowser.Margin, 20 + SampleBrowser.Margin / 2 + yOffset);
    }
}