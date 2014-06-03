using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    /// <summary>
    /// フォントクラス
    /// </summary>
    public class Font : IDestroy
    {
        internal swig.Font SwigObject { get; set; }

        internal Font(swig.Font swig)
        {
#if DEBUG
            // 唯一の対応するクラスであることを保証
            if (GC.Fonts.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
            SwigObject = swig;
        }

        ~Font()
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
    }
}
