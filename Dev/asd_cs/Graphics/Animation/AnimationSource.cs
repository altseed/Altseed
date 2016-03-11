using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// アニメーションの元データのクラス
    /// </summary>
    public class AnimationSource : IReleasable
    {
        internal swig.AnimationSource CoreInstance { get; set; }

        internal AnimationSource(swig.AnimationSource coreInstance)
        {
#if DEBUG
            // 唯一の対応するクラスであることを保証
            if (GC.AnimationSources.Contains(coreInstance.GetPtr()))
            {
                Particular.Helper.ThrowException("");
            }
#endif
            CoreInstance = coreInstance;
        }

        ~AnimationSource()
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
        /// アニメーションの長さ(60フレーム単位)を取得する。
        /// </summary>
        public float Length
        {
            get { return CoreInstance.GetLength(); }
        }

        /// <summary>
        /// ボーンごとのアニメーションを追加する。
        /// </summary>
        /// <param name="keyframeAnimation">アニメーション</param>
        public void AddAnimation(KeyframeAnimation keyframeAnimation)
        {
            CoreInstance.AddAnimation(keyframeAnimation.CoreInstance);
        }

        /// <summary>
        /// ボーンごとのアニメーションを削除する。
        /// </summary>
        /// <param name="keyframeAnimation">アニメーション</param>
        public void RemoveAnimation(KeyframeAnimation keyframeAnimation)
        {
            CoreInstance.RemoveAnimation(keyframeAnimation.CoreInstance);
        }
    }
}
