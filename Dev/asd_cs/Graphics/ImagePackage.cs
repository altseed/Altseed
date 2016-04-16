using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// 複数の画像が格納されているクラス
    /// </summary>
    public partial class ImagePackage : IReleasable
    {
        internal ImagePackage(swig.ImagePackage coreInstance)
        {
#if DEBUG
            // 唯一の対応するクラスであることを保証
            if (GC.ImagePackages.Contains(coreInstance.GetPtr()))
            {
                Particular.Helper.ThrowException("");
            }
#endif
            CoreInstance = coreInstance;
        }

        ~ImagePackage()
        {
            ForceToRelease();
        }

        public bool IsReleased
        {
            get
            {
                return CoreInstance == null;
            }
        }

        /// <summary>
        /// 強制的に使用しているメモリを開放する。
        /// </summary>
        /// <remarks>
        /// 何らかの理由でメモリが不足した場合に実行する。
        /// 開放した後の動作の保証はしていないので、必ず参照が残っていないことを確認する必要がある。
        /// </remarks>
        public void ForceToRelease()
        {
            lock (this)
            {
                if (CoreInstance == null) return;
                GC.Collector.AddObject(CoreInstance);
                CoreInstance = null;
            }
            Particular.GC.SuppressFinalize(this);
        }

        /// <summary>
        /// 画像を取得する。
        /// </summary>
        /// <param name="index">インデックス</param>
        /// <returns>画像</returns>
        public Texture2D GetImage(int index)
        {
            return GC.GenerateTexture2D(swig.Accessor.ImagePackage_GetImage(CoreInstance, index), GenerationType.Get);
        }
    }
}
