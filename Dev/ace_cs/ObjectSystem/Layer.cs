using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ace
{
	/// <summary>
	/// オブジェクトの更新と描画を管理するレイヤーの機能を提供する抽象クラス。
	/// </summary>
	public abstract class Layer
	{
		internal swig.CoreLayer commonObject = null;


		public bool IsUpdated { get; set; }

		public bool IsDrawn
		{
			get { return commonObject.GetIsDrawn(); }
			set { commonObject.SetIsDrawn(value); }
		}

		/// <summary>
		/// このインスタンスを管理している ace.Scene クラスのインスタンスを取得します。
		/// </summary>
		public Scene Scene { get; internal set; }

		/// <summary>
		/// このレイヤーの描画優先度を取得または設定します。この値が大きいほど手前に描画されます。
		/// </summary>
		public int DrawingPriority
		{
			get { return commonObject.GetDrawingPriority(); }
			set { commonObject.SetDrawingPriority(value); }
		}

		internal abstract void Update();

		internal abstract void DrawAdditionally();

		internal abstract void BeginDrawing();

		internal abstract void EndDrawing();

		internal swig.CoreLayer CoreLayer { get { return commonObject; } }


		protected virtual void OnUpdating()
		{
		}

		protected virtual void OnUpdated()
		{
		}

		protected virtual void OnDrawAdditionally()
		{
		}
	}
}
