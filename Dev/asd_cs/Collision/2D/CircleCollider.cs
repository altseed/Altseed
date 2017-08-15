using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd.swig;

namespace asd
{
    /// <summary>
    /// 図形としての円を扱うクラス。
    /// </summary>
    public class CircleCollider : Collider2D
    {
        internal override CoreCollider2D CoreCollider
        {
            get
            {
                return coreCollider;
            }

            set
            {
                coreCollider = value as swig.CoreCircleCollider;
            }
        }

        private swig.CoreCircleCollider coreCollider { get; set; }
        

        public CircleCollider()
        {
            coreCollider = Engine.ObjectSystemFactory.CreateCircleCollider();

            var p = coreCollider.GetPtr();

            if (GC.Collider2Ds.Contains(p))
            {
                Particular.Helper.ThrowException("");
            }

            GC.Collider2Ds.AddObject(p, this);
        }

        /// <summary>
        /// 円の中心座標を取得または設定する
        /// </summary>
        public Vector2DF Center
        {
            get
            {
                return coreCollider.GetCenter();
            }
            set
            {
                coreCollider.SetCenter(value);
            }
        }

        /// <summary>
        /// 円の半径を取得または設定する
        /// </summary>
        public float Radius
        {
            get
            {
                return coreCollider.GetRadius();
            }
            set
            {
                coreCollider.SetRadius(value);
            }
        }

        public override CoreCollider2DShapeType CoreCollider2DShapeType
        {
            get
            {
                return CoreCollider2DShapeType.Circle;
            }
        }
    }
}
