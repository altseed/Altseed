using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    public abstract class Shape : IDestroy
    {
        /// <summary>
        /// 図形のタイプを取得する。
        /// </summary>
        public abstract ShapeType ShapeType { get; }

        internal abstract swig.CoreShape CoreShape { get; }

        public abstract bool IsDestroyed { get; }

        public abstract void Destroy();
    }
}
