using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class SampleBrowser
{
    public static readonly float Margin = 16;

    private ISample[] samples;

    public ISample Selected = null;
    private SampleBrowserLayer browserLayer = null;
    private SampleInfoLayer infoLayer = null;

    public SampleBrowser(ISample[] samples)
    {
        this.samples = samples;
    }

    public void ShowInfo(ISample sample)
    {
        infoLayer.Show(sample);
    }

    public void Run()
    {
        while (true)
        {
            asd.Engine.Initialize("サンプルブラウザ", 640, 480, new asd.EngineOption());

            var scene = new asd.Scene();
            browserLayer = new SampleBrowserLayer(this, samples);

            var viewSize = SampleBrowserLayer.Columns * SampleBrowserLayer.ItemOffset.Y;
            var size = (480 - 80) * viewSize / browserLayer.TotalHeight;
            infoLayer = new SampleInfoLayer(size, browserLayer.TotalHeight, viewSize) { DrawingPriority = 2 };

            asd.Engine.ChangeScene(scene);
            scene.AddLayer(browserLayer);
            scene.AddLayer(infoLayer);

            var hintLayer = new asd.Layer2D();
            hintLayer.AddObject(new asd.TextureObject2D()
            {
                Texture = asd.Engine.Graphics.CreateTexture2D("Data/Browser/Hint.png")
            });

            scene.AddLayer(hintLayer);

            while (asd.Engine.DoEvents() && Selected == null)
            {
                asd.Engine.Update();
                infoLayer.MoveScrollBar(browserLayer.CameraArea.Y);
            }

            asd.Engine.Terminate();

            if (Selected == null)
            {
                break;
            }
            Selected.Run();
            Selected = null;
        }
    }
}