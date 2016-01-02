using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	public abstract partial class DrawnObject2D : Object2D
	{
		internal abstract swig.CoreDrawnObject2D CoreDrawnObject { get; }

		public abstract Color Color { get; set; }

		public abstract int DrawingPriority { get; set; }

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
			CoreDrawnObject.AddDrawnChild(child.CoreDrawnObject,
				(int)managementMode,
				(swig.ChildTransformingMode)transformingMode,
				(int)drawingMode);
			ChildrenList.Add(child);
			child.ParentInfo = new ParentInfo2D(this, managementMode);
		}
	}
}
