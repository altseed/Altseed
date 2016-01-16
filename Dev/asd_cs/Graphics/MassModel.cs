using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// 大量描画が可能な3Dモデルクラス
    /// </summary>
    public partial class MassModel : IReleasable
    {
        internal MassModel(swig.MassModel coreInstance)
        {
#if DEBUG
            // 唯一の対応するクラスであることを保証
            if (GC.MassModels.Contains(coreInstance.GetPtr()))
            {
                Particular.Helper.ThrowException("");
            }
#endif
            CoreInstance = coreInstance;
        }

        ~MassModel()
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
        /// 材質を設定する。
        /// </summary>
        /// <param name="material">材質</param>
        public void SetMaterial(Material3D material)
        {
            CoreInstance.SetMaterial(IG.GetMaterial3D(material));
        }
    }
}
