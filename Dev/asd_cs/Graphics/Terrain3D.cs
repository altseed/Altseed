using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// 地形のクラス
    /// </summary>
    public partial class Terrain3D : IReleasable
    {
        internal Terrain3D(swig.Terrain3D coreInstance)
        {
#if DEBUG
            // 唯一の対応するクラスであることを保証
            if (GC.Terrain3Ds.Contains(coreInstance.GetPtr()))
            {
                Particular.Helper.ThrowException("");
            }
#endif
            CoreInstance = coreInstance;
        }

        ~Terrain3D()
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
        public void SetMaterial(int materialIndex, Material3D material)
        {
            CoreInstance.SetMaterial(IG.GetMaterial3D(material));
        }

        /// <summary>
        /// メモリから地形のデータを読み込む。
        /// </summary>
        /// <param name="buffer">バッファ</param>
        /// <remarks>
        /// テクスチャのパスは保存されないので、読み込んだ後にAddSurfaceする必要がある。
        /// </remarks>
        public void LoadFromMemory(byte[] buffer)
        {
            var buf = new swig.VectorUint8();
            foreach (var b in buffer)
            {
                buf.Add(b);
            }

            CoreInstance.LoadFromMemory(buf);

            buf.Dispose();
        }

        /// <summary>
        /// メモリに地形のデータを保存する。
        /// </summary>
        /// <returns>地形のデータ</returns>
        public byte[] SaveToMemory()
        {
            var buf = CoreInstance.SaveToMemory();
            byte[] dst = new byte[buf.Count];
            buf.CopyTo(dst);

            return dst;
        }
    }
}
