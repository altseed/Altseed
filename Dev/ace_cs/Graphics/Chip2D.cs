using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    public class Chip2D : IDestroy
    {
        internal swig.Chip2D SwigObject { get; set; }

        internal Chip2D(swig.Chip2D swig)
        {
#if DEBUG
            // 唯一の対応するクラスであることを保証
            if (GC.Fonts.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
            SwigObject = swig;
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
            System.GC.SuppressFinalize(this);
        }

        Texture2D Texture
        {
            get { return GC.GenerateTexture2D(SwigObject.GetTexture(), GC.GenerationType.Get); }
            set { SwigObject.SetTexture(value.SwigObject); }
        }

        RectF Src
        {
            get { return SwigObject.GetSrc(); }
            set { SwigObject.SetSrc(value); }
        }

        Color Color
        {
            get { return SwigObject.GetColor(); }
            set { SwigObject.SetColor(value); }
        }

        bool TurnLR
        {
            get { return SwigObject.GetTurnLR(); }
            set { SwigObject.SetTurnLR(value); }
        }

        bool TurnUL
        {
            get { return SwigObject.GetTurnUL(); }
            set { SwigObject.SetTurnUL(value); }
        }

        AlphaBlendMode AlphaBlendMode
        {
            get { return (AlphaBlendMode)SwigObject.GetAlphaBlendMode(); }
            set { SwigObject.SetAlphaBlendMode((swig.eAlphaBlend)value); }
        }
    }
}
