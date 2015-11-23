using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace sample_cs
{
	class SampleItem : TextureObject2D
	{
		public static readonly Vector2DF Size = new Vector2DF(150, 112.5f);
		public static int TitleMaxLength = 19;

		private RectangleShape shape_;
		private TextureObject2D frame { get; set; }

		public ISample Sample { get; private set; }
		public RectangleShape Shape
		{
			get { return shape_ ?? GetArea(); }
		}
		public TextObject2D Title { get; private set; }

		public new Vector2DF Position
		{
			get { return base.Position; }
			set
			{
				base.Position = value;
				shape_ = null;
			}
		}

		public SampleItem(ISample sample, Font font)
		{
			var sampleName = sample.GetType().Name;
			Sample = sample;

			Texture = Engine.Graphics.CreateTexture2D("Data/Browser/" + sampleName + ".png");
			if(Texture == null)
			{
				Texture = Engine.Graphics.CreateTexture2D("Data/Browser/Default.png");
			}
			Scale = Size / Texture.Size.To2DF();

			var text = sampleName;
			if(text.Length > TitleMaxLength)
			{
				text = "…" + text.Substring(text.Length - TitleMaxLength);
			}

			Title = new TextObject2D()
			{
				Text = text,
				Font = font,
				Position = new Vector2DF(0, 115),
				DrawingPriority = 1,
			};
			AddChild(Title, ChildMode.Position);

			frame = new TextureObject2D()
			{
				Texture = Engine.Graphics.CreateTexture2D("Data/Browser/Frame.png"),
				Position = new Vector2DF(-3, -3),
			};
			AddChild(frame, ChildMode.Position);
		}

		public RectangleShape GetArea()
		{
			return new RectangleShape
			{
				DrawingArea = new RectF(Position.X, Position.Y, Size.X, Size.Y),
			};
		}

		protected override void OnStart()
		{
			Layer.AddObject(Title);
			Layer.AddObject(frame);
		}

		protected override void OnVanish()
		{
			Title.Vanish();
			frame.Vanish();
		}

		public void Activate()
		{
			frame.Texture = Engine.Graphics.CreateTexture2D("Data/Browser/FrameActive.png");
		}

		public void Disactivate()
		{
			frame.Texture = Engine.Graphics.CreateTexture2D("Data/Browser/Frame.png");
		}
	}
}
