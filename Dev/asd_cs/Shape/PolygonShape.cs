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
        }
        private swig.CorePolygonShape corePolygon { get; set; }

        public override ShapeType ShapeType
        {
            get { return ShapeType.PolygonShape; }
        }

        public PolygonShape()
            : base()
        {
            corePolygon = Engine.ObjectSystemFactory.CreatePolygonShape();

            var p = corePolygon.GetPtr();
            if (GC.Shapes.GetObject(p) != null)
            {
				Particular.Helper.ThrowException("");
            }
            GC.Shapes.AddObject(p, this);
        }

        #region GC対応
        ~PolygonShape()
        {
            ForceToRelease();
        }

        public override bool IsReleased
        {
            get { return corePolygon == null; }
        }

		/// <summary>
		/// 強制的に使用しているメモリを開放する。
		/// </summary>
		/// <remarks>
		/// 何らかの理由でメモリが不足した場合に実行する。
		/// 開放した後の動作の保証はしていないので、必ず参照が残っていないことを確認する必要がある。
		/// </remarks>
        public override void ForceToRelease()
        {
            lock (this)
            {
                if (corePolygon == null) return;
                GC.Collector.AddObject(corePolygon);
                corePolygon = null;
            }
            Particular.GC.SuppressFinalize(this);
        }
        #endregion

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
