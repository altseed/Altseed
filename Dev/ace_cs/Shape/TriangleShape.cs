using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    public class TriangleShape : Shape
    {
        internal override swig.CoreShape CoreShape
        {
            get { return coreTriangle; }
        }
        private swig.CoreTriangleShape coreTriangle { get; set; }

        public override ShapeType ShapeType
        {
            get { return ShapeType.TriangleShape; }
        }

        public TriangleShape()
            : base()
        {
            coreTriangle = Engine.ObjectSystemFactory.CreateTriangleShape();

            var p = coreTriangle.GetPtr();
            if (GC.Shapes.GetObject(p) != null)
            {
                throw new Exception();
            }
            GC.Shapes.AddObject(p, this);
        }

        #region GC対応
        ~TriangleShape()
        {
            Destroy();
        }

        public override bool IsDestroyed
        {
            get { return coreTriangle == null; }
        }

        public override void Destroy()
        {
            lock (this)
            {
                if (coreTriangle == null) return;
                GC.Collector.AddObject(coreTriangle);
                coreTriangle = null;
            }
            Particular.GC.SuppressFinalize(this);
        }
        #endregion

        /// <summary>
        /// 指定したインデックスの頂点の座標を取得する。
        /// </summary>
        /// <param name="index">座標を取得する頂点のインデックス（0 &lt;= index &lt; 3）</param>
        /// <returns>頂点の座標</returns>
        public Vector2DF GetPointByIndex(int index)
        {
            return coreTriangle.GetPointByIndex(index);
        }

        /// <summary>
        /// 指定したインデックスの頂点の座標を設定する。
        /// </summary>
        /// <param name="point">インデックスで指定した頂点に設定する座標</param>
        /// <param name="index">座標を設定する頂点のインデックス（0 &lt;= index &lt; 3）</param>
		public void SetPointByIndex(Vector2DF point, int index)
        {
            coreTriangle.SetPointByIndex(point, index);
        }

        /// <summary>
        /// 指定したインデックスの頂点のUVを取得する。
        /// </summary>
        /// <param name="index">UVを取得する頂点のインデックス（0 &lt;= index &lt; 3）</param>
        /// <returns>頂点のUV</returns>
		public Vector2DF GetUVByIndex(int index)
        {
            return coreTriangle.GetUVByIndex(index);
        }

        /// <summary>
        /// 指定したインデックスの頂点のUVを設定する。
        /// </summary>
        /// <param name="uv">インデックスで指定した頂点に設定するUV</param>
        /// <param name="index">UVを設定する頂点のインデックス（0 &lt;= index &lt; 3）</param>
		public void SetUVByIndex(Vector2DF uv, int index)
        {
            coreTriangle.SetUVByIndex(uv, index);
        }
    }
}
