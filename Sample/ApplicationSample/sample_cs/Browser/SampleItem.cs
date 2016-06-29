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
        public static int TitleMaxLength = 9;

        private RectangleShape shape_;
        private TextureObject2D frame { get; set; }

        public ISample Sample { get; private set; }
        public RectangleShape Shape
        {
            get
            {
                if (shape_ == null)
                {
                    shape_ = GetArea();
                }
                return shape_;
            }

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
            Sample = sample;

            Texture = Engine.Graphics.CreateTexture2D("Data/Browser/" + sample.GetType().Name + ".png");
            if (Texture == null)
            {
                Texture = Engine.Graphics.CreateTexture2D("Data/Browser/Default.png");
            }
            Scale = Size / Texture.Size.To2DF();

            string text = "";
            if (sample.Title != "")
            {
                text = sample.Title;
            }
            else
            {
                text = sample.GetType().Name;
            }

            Title = new TextObject2D()
            {
                Text = GetWrappedString(font, text),
                Font = font,
                Position = new Vector2DF(0, 115),
                DrawingPriority = 1,
            };
            AddChild(Title, ChildManagementMode.Nothing, ChildTransformingMode.Position);

            frame = new TextureObject2D()
            {
                Texture = Engine.Graphics.CreateTexture2D("Data/Browser/Frame.png"),
                Position = new Vector2DF(-3, -3),
            };
            AddChild(frame, ChildManagementMode.Nothing, ChildTransformingMode.Position);
        }

        public RectangleShape GetArea()
        {
            return new RectangleShape
            {
                DrawingArea = new RectF(Position.X, Position.Y, Size.X, Size.Y),
            };
        }

        private string GetWrappedString(Font font, string title)
        {
            string result = title;
            if (font.CalcTextureSize(result, WritingDirection.Horizontal).X <= Size.X)
            {
                return result;
            }
            while (font.CalcTextureSize(result + "…", WritingDirection.Horizontal).X > Size.X)
            {
                result = result.Substring(0, result.Length - 1);
            }
            return result + "…";
        }

        protected override void OnAdded()
        {
            Layer.AddObject(Title);
            Layer.AddObject(frame);
        }

        protected override void OnDispose()
        {
            Title.Dispose();
            frame.Dispose();
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
