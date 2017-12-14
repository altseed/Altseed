using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// 
    /// </summary>
    public class Collision2DInfo
    {
        Collider2D selfCollider;
        Collider2D theirsCollider;
        CollisionType collisionType;

        internal Collision2DInfo(Collider2D selfCollider, Collider2D theirsCollider, CollisionType collisionType)
        {
            this.selfCollider = selfCollider;
            this.theirsCollider = theirsCollider;
            this.collisionType = collisionType;
        }

        /// <summary>
        /// 衝突した自分自身のコライダを取得する
        /// </summary>
        public Collider2D SelfCollider
        {
            get
            {
                return selfCollider;
            }
        }

        /// <summary>
        /// 衝突した相手のコライダを取得する
        /// </summary>
        public Collider2D TheirsCollider
        {
            get
            {
                return theirsCollider;
            }
        }

        /// <summary>
        /// 衝突状態の種類を取得する
        /// </summary>
        public CollisionType CollisionType
        {
            get
            {
                return collisionType;
            }
        }
    }
}
