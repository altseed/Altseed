using asd.swig;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// ポリゴンのコライダクラス
    /// </summary>
    public class PolygonCollider : Collider2D
    {
        internal override CoreCollider2D CoreCollider
        {
            get
            {
                return coreCollider;
            }

            set
            {
                coreCollider = value as swig.CorePolygonCollider;
            }
        }

        private swig.CorePolygonCollider coreCollider { get; set; }


        public PolygonCollider()
        {
            coreCollider = Engine.ObjectSystemFactory.CreatePolygonCollider();

            var p = coreCollider.GetPtr();

            if (GC.Collider2Ds.Contains(p))
            {
                Particular.Helper.ThrowException("");
            }

            GC.Collider2Ds.AddObject(p, this);
        }

        /// <summary>
        /// このポリゴンが正当か否かを取得する
        /// </summary>
        public bool IsValid
        {
            get
            {
                return coreCollider.GetIsValid();
            }
        }

        /// <summary>
        /// このポリゴンに新たな頂点を追加する
        /// </summary>
        public void AddVertex(Vector2DF vertex)
        {
            coreCollider.AddVertex(vertex);
        }

        /// <summary>
        /// このポリゴンの頂点全てをクリアする
        /// </summary>
        public void ClearVertexes()
        {
            coreCollider.ClearVertexes();
        }

        /// <summary>
        /// このポリゴンの頂点数を取得する
        /// </summary>
        public int VertexesNum
        {
            get
            {
                return coreCollider.GetVertexesNum();
            }
        }

        /// <summary>
        /// このポリゴンのindex番目のポリゴンを取得する
        /// </summary>
        /// <param name="index">ポリゴンの頂点のインデックス</param>
        /// <returns>取得したポリゴンの頂点座標</returns>
        public Vector2DF GetVertex(int index)
        {
            return coreCollider.GetVertex(index);
        }

        /// <summary>
        /// このポリゴンのindex番目のポリゴンを設定する
        /// </summary>
        /// <param name="index">ポリゴンの頂点のインデックス</param>
        /// <param name="vertex">設定する頂点座標</param>
        public void SetVertex(int index, Vector2DF vertex)
        {
            coreCollider.SetVertex(index, vertex);
        }

        public override CoreCollider2DShapeType CoreCollider2DShapeType
        {
            get
            {
                return CoreCollider2DShapeType.Polygon;
            }
        }
    }
}