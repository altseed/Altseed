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
        ArcShape = swig.ShapeType.ArcShape,

        /// <summary>
        /// 円
        /// </summary>
        CircleShape = swig.ShapeType.CircleShape,

        /// <summary>
        /// 矩形
        /// </summary>
        RectangleShape = swig.ShapeType.RectangleShape,

        /// <summary>
        /// 線分
        /// </summary>
        LineShape = swig.ShapeType.LineShape,

        /// <summary>
        /// 三角形
        /// </summary>
        TriangleShape = swig.ShapeType.TriangleShape,

        /// <summary>
        /// 多角形
        /// </summary>
        PolygonShape = swig.ShapeType.PolygonShape,
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
