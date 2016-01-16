using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// キューブマップ
    /// </summary>
    public class CubemapTexture : Texture, IReleasable
    {
        internal swig.CubemapTexture CoreInstance { get; set; }

        internal CubemapTexture(swig.CubemapTexture coreInstance)
        {
#if DEBUG
            // 唯一の対応するクラスであることを保証
            if (GC.CubemapTextures.Contains(coreInstance.GetPtr()))
            {
                Particular.Helper.ThrowException("");
            }
#endif
            CoreInstance = coreInstance;
        }

        ~CubemapTexture()
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
        /// ミップマップ数
        /// </summary>
        public int MipmapCount
        {
            get
            {
                return CoreInstance.GetMipmapCount();
            }
        }
    }
}
