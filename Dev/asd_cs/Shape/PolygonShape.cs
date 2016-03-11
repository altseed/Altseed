using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{

    /// <summary>
    /// 図形としての多角形を扱うクラス。
    /// </summary>
    public class PolygonShape : Shape
    {
        internal override swig.CoreShape CoreShape
        {
            get { return corePolygon; }
            set { corePolygon = value as swig.CorePolygonShape; }
        }
        private swig.CorePolygonShape corePolygon { get; set; }

        public override ShapeType ShapeType
        {
            get { return ShapeType.PolygonShape; }
        }

        public PolygonShape()
        {
            corePolygon = Engine.ObjectSystemFactory.CreatePolygonShape();

            var p = corePolygon.GetPtr();

            if (GC.Shapes.Contains(p))
            {
                Particular.Helper.ThrowException("");
            }

            GC.Shapes.AddObject(p, this);
        }

        /// <summary>
        /// 多角形を構成する頂点を追加する。
        /// </summary>
        /// <param name="vertex">新規に追加する頂点の座標</param>
        public void AddVertex(Vector2DF vertex)
        {
            corePolygon.AddVertex(vertex);
        }

        /// <summary>
        /// 多角形を構成する頂点を全て削除する。
        /// </summary>
        public void ClearVertexes()
        {
            corePolygon.ClearVertexes();
        }

        /// <summary>
        /// 多角形を構成する頂点の数量を取得する。
        /// </summary>
        /// <returns>多角形を構成する頂点の数量</returns>
        public int GetVertexesNum()
        {
            return corePolygon.GetVertexesNum();
        }
    }
}
