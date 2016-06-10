using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// 描画に関するパラメータを持つ2Dオブジェクト。
    /// </summary>
	public abstract partial class DrawnObject2D : Object2D
	{
		internal abstract swig.CoreDrawnObject2D CoreDrawnObject { get; }

        /// <summary>
        /// この2Dオブジェクトを描画する際に合成する色を取得または設定する。
        /// </summary>
		public abstract Color Color { get; set; }

        /// <summary>
        /// この2Dオブジェクトを描画する際の描画優先度を取得または設定する。描画優先度が高いほど手前に描画される。
        /// </summary>
		public abstract int DrawingPriority { get; set; }

        /// <summary>
        /// 親子関係を考慮して最終的に描画時にこのオブジェクトに合成する色を取得する。
        /// </summary>
        public Color AbsoluteColor
        {
            get { return CoreDrawnObject.GetAbsoluteColor(); }
        }

        /// <summary>
        /// 親子関係を考慮したこのオブジェクトの最終的な描画優先度を取得する。
        /// </summary>
        public int AbsoluteDrawingPriority
        {
            get { return CoreDrawnObject.GetAbsoluteDrawingPriority(); }
        }

		/// <summary>
		/// 描画に関する同期設定を指定して、指定した2Dオブジェクトを子オブジェクトとしてこのインスタンスに追加する。
		/// </summary>
		/// <param name="child">追加する子オブジェクト</param>
		/// <param name="managementMode">子オブジェクトの管理に関する同期設定。フラグをOR演算でつなげて複数指定することができる。</param>
		/// <param name="transformingMode">子オブジェクトの変形に関する同期設定。</param>
		/// <param name="drawingMode">子オブジェクトの描画に関する同期設定。フラグをOR演算でつなげて複数指定することができる。</param>
		public void AddDrawnChild(DrawnObject2D child,
			ChildManagementMode managementMode,
			ChildTransformingMode transformingMode,
			ChildDrawingMode drawingMode)
		{
			var e = new EventToMangeDrawnFamilyship2D(this, child);
			e.SetUpAsAddEvent(managementMode, transformingMode, drawingMode);
			Engine.ChangesToBeCommited.Enqueue(e);

		}

	    internal void ImmediatelyAddDrawnChild(
		    DrawnObject2D child,
		    ChildManagementMode managementMode,
		    ChildTransformingMode transformingMode,
		    ChildDrawingMode drawingMode)
		{
			CoreDrawnObject.AddDrawnChild(child.CoreDrawnObject,
				asd.Particular.ChildManagementMode.ToInt(managementMode),
				(swig.ChildTransformingMode)transformingMode,
				asd.Particular.ChildDrawingMode.ToInt(drawingMode));
			ChildrenList.Add(child);
			child.ParentInfo = new ParentInfo2D(this, managementMode);
		}
	}
}
