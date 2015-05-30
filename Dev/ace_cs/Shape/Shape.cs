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

        /// <summary>
        /// 指定した図形と衝突しているか否かを調べる。
        /// </summary>
        /// <param name="shape">衝突を調べる図形</param>
        /// <returns>衝突したか否か</returns>
        public bool GetIsCollidedWith(Shape shape)
        {
            return CoreShape.GetIsCollidedWith(shape.CoreShape);
        }
    }
}
