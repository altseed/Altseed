using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// 2Dコライダの形状タイプ
    /// </summary>
    public enum CoreCollider2DShapeType
    {
        /// <summary>
        /// 円形
        /// </summary>
        Circle = swig.Collider2DShapeType.Circle,

        /// <summary>
        /// 矩形
        /// </summary>
        Rectangle = swig.Collider2DShapeType.Rectangle,

        /// <summary>
        /// 線形
        /// </summary>
        Line = swig.Collider2DShapeType.Line,

        /// <summary>
        /// ポリゴン
        /// </summary>
        Polygon = swig.Collider2DShapeType.Polygon
    }
}
