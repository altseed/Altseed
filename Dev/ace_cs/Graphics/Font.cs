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
    public partial class Font : IDestroy
    {
        internal Font(swig.Font swig)
        {
#if DEBUG
            // 唯一の対応するクラスであることを保証
			if (GC.Fonts.GetObject(swig.GetPtr()) != null) Particular.Helper.ThrowException("");
#endif
            CoreInstance = swig;
        }

        ~Font()
        {
            Destroy();
        }

        public bool IsDestroyed
        {
            get
            {
                return CoreInstance == null;
            }
        }

        public void Destroy()
        {
            lock (this)
            {
                if (CoreInstance == null) return;
                GC.Collector.AddObject(CoreInstance);
                CoreInstance = null;
            }
            Particular.GC.SuppressFinalize(this);
        }
    }
}
