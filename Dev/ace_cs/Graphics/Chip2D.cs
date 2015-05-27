using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    public class Chip2D : IDestroy
    {
        internal swig.CoreChip2D SwigObject { get; set; }

        public Chip2D()
        {
            SwigObject = Engine.ObjectSystemFactory.CreateChip2D();

            var p = SwigObject.GetPtr();
            if (GC.Shapes.GetObject(p) != null)
            {
                Particular.Helper.ThrowException("");
            }
            GC.Chip2Ds.AddObject(p, this);
        }

        ~Chip2D()
        {
            Destroy();
        }

        public bool IsDestroyed
        {
            get
            {
                return SwigObject == null;
            }
        }

        public void Destroy()
        {
            lock (this)
            {
                if (SwigObject == null) return;
                GC.Collector.AddObject(SwigObject);
                SwigObject = null;
            }
            Particular.GC.SuppressFinalize(this);
        }

        public Texture2D Texture
        {
            get { return GC.GenerateTexture2D(swig.Accessor.Chip2D_GetTexture(SwigObject) , GC.GenerationType.Get); }
            set { SwigObject.SetTexture(IG.GetTexture2D(value)); }
        }

        public RectF Src
        {
            get { return SwigObject.GetSrc(); }
            set { SwigObject.SetSrc(value); }
        }

        public Color Color
        {
            get { return SwigObject.GetColor(); }
            set { SwigObject.SetColor(value); }
        }

        public bool TurnLR
        {
            get { return SwigObject.GetTurnLR(); }
            set { SwigObject.SetTurnLR(value); }
        }

        public bool TurnUL
        {
            get { return SwigObject.GetTurnUL(); }
            set { SwigObject.SetTurnUL(value); }
        }

        public AlphaBlendMode AlphaBlendMode
        {
            get { return (AlphaBlendMode)SwigObject.GetAlphaBlendMode(); }
            set { SwigObject.SetAlphaBlendMode((swig.AlphaBlendMode)value); }
        }
    }
}
