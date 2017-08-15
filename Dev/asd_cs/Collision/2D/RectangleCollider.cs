using asd.swig;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// 矩形のコライダクラス
    /// </summary>
    public class RectangleCollider : Collider2D
    {
        internal override CoreCollider2D CoreCollider
        {
            get
            {
                return coreCollider;
            }

            set
            {
                coreCollider = value as swig.CoreRectangleCollider;
            }
        }

        private swig.CoreRectangleCollider coreCollider { get; set; }


        public RectangleCollider()
        {
            coreCollider = Engine.ObjectSystemFactory.CreateRectangleCollider();

            var p = coreCollider.GetPtr();

            if (GC.Collider2Ds.Contains(p))
            {
                Particular.Helper.ThrowException("");
            }

            GC.Collider2Ds.AddObject(p, this);
        }

        /// <summary>
        /// 矩形の範囲を取得または設定する
        /// </summary>
        public RectF Area
        {
            get
            {
                return coreCollider.GetArea();
            }
            set
            {
                coreCollider.SetArea(value);
            }
        }
        

        public override CoreCollider2DShapeType CoreCollider2DShapeType
        {
            get
            {
                return CoreCollider2DShapeType.Rectangle;
            }
        }
    }
}
