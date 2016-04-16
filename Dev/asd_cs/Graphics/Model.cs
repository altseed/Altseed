using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// 3Dモデルクラス
    /// </summary>
    public partial class Model : IReleasable
    {
        internal Model(swig.Model coreInstance)
        {
#if DEBUG
            // 唯一の対応するクラスであることを保証
            if (GC.Models.Contains(coreInstance.GetPtr()))
            {
                Particular.Helper.ThrowException("");
            }
#endif
            CoreInstance = coreInstance;
        }

        ~Model()
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
        /// モデルが持つアニメーションクリップを取得する。
        /// </summary>
        /// <param name="index">アニメーションクリップのインデックス</param>
        /// <returns>アニメーションクリップ</returns>
        public AnimationClip GetAnimationClip(int index)
        {
            return GC.GenerateAnimationClip(swig.Accessor.Model_GetAnimationClip(CoreInstance, index), GenerationType.Get);
        }

        /// <summary>
        /// メッシュを取得する。
        /// </summary>
        /// <param name="index">メッシュのインデックス</param>
        /// <returns></returns>
        public Mesh GetMesh(int index)
        {
            return GC.GenerateMesh(swig.Accessor.Model_GetMesh(CoreInstance, index), GenerationType.Get);
        }
    }
}
