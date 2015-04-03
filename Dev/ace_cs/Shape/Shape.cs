using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    /// <summary>
    /// 図形の種類
    /// </summary>
    public enum ShapeType
    {
        /// <summary>
        /// 弧
        /// </summary>
        Arc = swig.ShapeType.Arc,

        /// <summary>
        /// 円
        /// </summary>
        Circle = swig.ShapeType.Circle,

        /// <summary>
        /// 矩形
        /// </summary>
        Rectangle = swig.ShapeType.Rectangle,

        /// <summary>
        /// 線分
        /// </summary>
        Line = swig.ShapeType.Line,

        /// <summary>
        /// 三角形
        /// </summary>
        Triangle = swig.ShapeType.Triangle,
    }

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
