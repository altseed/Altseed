using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{

    /// <summary>
    /// 図形としての線分を扱うクラス。
    /// </summary>
    public class LineShape : Shape,IDestroy
    {
        internal override swig.CoreShape CoreShape
        {
            get { return coreLine; }
        }
        private swig.CoreLineShape coreLine { get; set; }

        public override ShapeType ShapeType
        {
            get { return ShapeType.LineShape; }
        }

		public LineShape()
			: base()
		{
			coreLine = Engine.ObjectSystemFactory.CreateLineShape();

			var p = coreLine.GetPtr();
			if (GC.Shapes.GetObject(p) != null)
			{
				Particular.Helper.ThrowException("");
			}
			GC.Shapes.AddObject(p, this);
		}

        #region GC対応
        ~LineShape()
        {
            Destroy();
        }

        public override bool IsDestroyed
        {
            get { return coreLine == null; }
        }

		/// <summary>
		/// 強制的に使用しているメモリを開放する。
		/// </summary>
		/// <remarks>
		/// 何らかの理由でメモリが不足した場合に実行する。
		/// 開放した後の動作の保証はしていないので、必ず参照が残っていないことを確認する必要がある。
		/// </remarks>
        public override void Destroy()
        {
            lock (this)
            {
                if (coreLine == null) return;
                GC.Collector.AddObject(coreLine);
                coreLine = null;
            }
            Particular.GC.SuppressFinalize(this);
        }
        #endregion


        /// <summary>
        /// 線分の始点座標を取得もしくは設定する。
        /// </summary>
        public Vector2DF StartingPosition
        {
            get
            {
                return coreLine.GetStartingPosition();
            }
            set
            {
                coreLine.SetStartingPosition(value);
            }
        }

        /// <summary>
        /// 線分の終点座標を取得もしくは設定する。
        /// </summary>
        public Vector2DF EndingPosition
        {
            get
            {
                return coreLine.GetEndingPosition();
            }
            set
            {
                coreLine.SetEndingPosition(value);
            }
        }

        /// <summary>
        /// 線分の太さを取得もしくは設定する。
        /// </summary>
        public float Thickness
        {
            get
            {
                return coreLine.GetThickness();
            }
            set
            {
                coreLine.SetThickness(value);
            }
        }
    }
}
