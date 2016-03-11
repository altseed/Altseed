using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// 破棄可能なインスタンス
    /// </summary>
    public interface IReleasable
    {
        /// <summary>
        /// 破棄されているか？
        /// </summary>
        bool IsReleased { get; }

        /// <summary>
        /// 強制的に使用しているメモリを開放する。
        /// </summary>
        /// <remarks>
        /// 何らかの理由でメモリが不足した場合に実行する。
        /// 開放した後の動作の保証はしていないので、必ず参照が残っていないことを確認する必要がある。
        /// </remarks>
        void ForceToRelease();
    }
}
