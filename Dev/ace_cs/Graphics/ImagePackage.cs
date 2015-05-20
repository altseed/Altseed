using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
    /// 複数の画像が格納されているクラス
    /// </summary>
    public partial class ImagePackage : IDestroy
    {
        internal ImagePackage (swig.ImagePackage  swig)
        {
#if DEBUG
            // 唯一の対応するクラスであることを保証
            if (GC.ImagePackages.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
            CoreInstance = swig;
        }

        ~ImagePackage ()
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
            System.GC.SuppressFinalize(this);
        }

		/// <summary>
		/// 画像を取得する。
		/// </summary>
		/// <param name="index">インデックス</param>
		/// <returns>画像</returns>
		public Texture2D GetImage(int index)
		{
			return GC.GenerateTexture2D(swig.Accessor.ImagePackage_GetImage(CoreInstance,index), GC.GenerationType.Get);
		}
    }
}
