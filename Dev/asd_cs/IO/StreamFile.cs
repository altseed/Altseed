using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;


namespace asd
{
    public partial class StreamFile : IReleasable
    {
        internal StreamFile(swig.StreamFile coreInstance)
        {
#if DEBUG
            if (GC.StreamFiles.Contains(coreInstance.GetPtr()))
            {
                Particular.Helper.ThrowException("");
            }
#endif

            CoreInstance = coreInstance;
        }

        ~StreamFile()
        {
            ForceToRelease();
        }

        /// <summary>
        /// 
        /// </summary>
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

        public unsafe void Read(List<byte> buffer, int size)
        {
            if (buffer == null) return;

			asd.Particular.Helper.CopyStreamFileToList(CoreInstance, buffer, size);
        }
    }
}
