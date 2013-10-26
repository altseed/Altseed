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
		/// <summary>
		/// このインスタンスを管理している ace.Scene クラスのインスタンスを取得します。
		/// </summary>
		public abstract Scene Scene { get; internal set; }

		/// <summary>
		/// このレイヤーの描画優先度を取得または設定します。この値が大きいほど手前に描画されます。
		/// </summary>
		public abstract int DrawingPriority { get; set; }

		internal abstract void Update();

		internal abstract void DrawAdditionally();

		internal abstract void BeginDrawing();

		internal abstract void EndDrawing();

		internal abstract unsafe swig.CoreLayer CoreLayer { get; }
	}
}
