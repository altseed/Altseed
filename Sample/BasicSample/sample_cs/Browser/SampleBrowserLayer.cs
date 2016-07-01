using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

class SampleBrowserLayer : Layer2D
{
	public static readonly int Columns = 3;
	public static readonly Vector2DF ItemOffset = new Vector2DF(632 / Columns, 150);

	private List<SampleItem> items;
	private CameraObject2D camera;
	private SampleItem activeItem;
	private SampleBrowser browser;

	public RectF CameraArea
	{
		get { return camera.Src.ToF(); }
	}
	public float TotalHeight { get; private set; }

	public SampleBrowserLayer(SampleBrowser browser, ISample[] samples)
	{
		Name = "BrowserLayer";
		this.browser = browser;
		items = new List<SampleItem>();

		var font = Engine.Graphics.CreateDynamicFont("", 12, new Color(255, 255, 255, 255), 1, new Color(0, 0, 0, 255));
		int index = 0;
		foreach (var sample in samples)
		{
			var item = new SampleItem(sample, font);
			var x = ItemOffset.X * (index % Columns) + 8;
			var y = ItemOffset.Y * (index / Columns) + 8 + 20;
			item.Position = new Vector2DF(x, y);
			AddObject(item);
			items.Add(item);

			index++;
		}

		var isThereJut = index % Columns == 0;

		if (isThereJut)
		{
			TotalHeight = ItemOffset.Y * (index / Columns) + 8 + 20;
		}
		else
		{
			TotalHeight = ItemOffset.Y * (index / Columns + 1) + 8 + 20;
		}


		camera = new CameraObject2D()
		{
			Src = new RectI(0, 0, 640, 480),
			Dst = new RectI(0, 0, 640, 480),
		};
		AddObject(camera);
	}

	protected override void OnUpdated()
	{
		var rows = items.Count / Columns;

		if (items.Count % Columns != 0)
		{
			rows++;
		}

		var y = camera.Src.Y - Engine.Mouse.MiddleButton.WheelRotation * 30;
		y = Math.Max(0, y);
		y = Math.Min(rows * ItemOffset.Y - 480 + 24 + 60, y);
		camera.Src = new RectI(
			camera.Src.X,
			(int)y,
			camera.Src.Width,
			camera.Src.Height);

		var mouse = new CircleShape()
		{
			Position = Engine.Mouse.Position + new Vector2DF(0, camera.Src.Y),
			OuterDiameter = 2,
		};

		if (activeItem != null && !activeItem.Shape.GetIsCollidedWith(mouse))
		{
			activeItem.Disactivate();
			activeItem = null;
			browser.ShowInfo(null);
		}

		foreach (var item in items)
		{
			if (item.Shape.GetIsCollidedWith(mouse))
			{
				if (item != activeItem)
				{
					item.Activate();
					activeItem = item;
					browser.ShowInfo(item.Sample);
				}
				if (Engine.Mouse.LeftButton.ButtonState == MouseButtonState.Push)
				{
					browser.Selected = item.Sample;
				}
				break;
			}
		}
	}
}
