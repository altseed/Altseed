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

        /// <summary>
        /// 描画テキストと描画方向を与えると、その文字の描画領域を返す。
        /// </summary>
        /// <param name="text">描画テキスト</param>
        /// <param name="writingDirection">描画方向</param>
        /// <returns>文字の描画領域</returns>
        public Vector2DI CalcTextureSize(string text, WritingDirection writingDirection)
        {
            return SwigObject.CalcTextureSize(text, (ace.swig.WritingDirection)writingDirection);
        }
            
    }
}
