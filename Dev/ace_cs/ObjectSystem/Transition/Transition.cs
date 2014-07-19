using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 画面遷移時の効果を適用するクラス
	/// </summary>
	public class Transition : IDestroy
	{
		internal swig.CoreTransition SwigObject;

		public Transition()
		{
			SwigObject = Engine.ObjectSystemFactory.CreateTransition();

			var p = SwigObject.GetPtr();
			if (GC.Transitions.GetObject(p) != null)
			{
				throw new Exception();
			}
			GC.Transitions.AddObject(p, this);
		}

		#region GC対応
		~Transition()
		{
			Destroy();
		}

		public bool IsDestroyed
		{
			get { return SwigObject == null; }
		}

		public void Destroy()
		{
			lock (this)
			{
				if (SwigObject == null) return;
				GC.Collector.AddObject(SwigObject);
				SwigObject = null;
			}
			System.GC.SuppressFinalize(this);
		}
		#endregion

		/// <summary>
		/// オーバーライドして、このオブジェクトの更新処理を記述できる。
		/// </summary>
		internal protected virtual void OnUpdate() { }

		/// <summary>
		/// 実際にシーンを次のシーンに切り替える。
		/// </summary>
		public void ChangeScene()
		{
			SwigObject.ChangeScene();
		}

		/// <summary>
		/// 画面遷移処理を終了する。
		/// </summary>
		public void Finish()
		{
			SwigObject.Finish();
		}

		/// <summary>
		/// 次の画面を3角形に貼り付けて描画する。
		/// </summary>
		/// <param name="pos1">座標1</param>
		/// <param name="uv1">UV1</param>
		/// <param name="col1">頂点カラー1</param>
		/// <param name="pos2">座標2</param>
		/// <param name="uv2">UV2</param>
		/// <param name="col2">頂点カラー2</param>
		/// <param name="pos3">座標3</param>
		/// <param name="uv3">UV3</param>
		/// <param name="col3">頂点カラー3</param>
		/// <remarks>ChangeSceneを実行するまでは無効である。</remarks>
		public void DrawNextScene(
		Vector2DF pos1, Vector2DF uv1, Color col1,
		Vector2DF pos2, Vector2DF uv2, Color col2,
		Vector2DF pos3, Vector2DF uv3, Color col3)
		{
			SwigObject.DrawNextScene(
				pos1, uv1, col1,
				pos2, uv2, col2,
				pos3, uv3, col3);
		}

		/// <summary>
		/// 前の画面を3角形に貼り付けて描画する。
		/// </summary>
		/// <param name="pos1">座標1</param>
		/// <param name="uv1">UV1</param>
		/// <param name="col1">頂点カラー1</param>
		/// <param name="pos2">座標2</param>
		/// <param name="uv2">UV2</param>
		/// <param name="col2">頂点カラー2</param>
		/// <param name="pos3">座標3</param>
		/// <param name="uv3">UV3</param>
		/// <param name="col3">頂点カラー3</param>
		/// <remarks>ChangeSceneを実行すると前のシーンの描画は停止する。</remarks>
		public void DrawPreviousScene(
		Vector2DF pos1, Vector2DF uv1, Color col1,
		Vector2DF pos2, Vector2DF uv2, Color col2,
		Vector2DF pos3, Vector2DF uv3, Color col3)
		{
			SwigObject.DrawPreviousScene(
				pos1, uv1, col1,
				pos2, uv2, col2,
				pos3, uv3, col3);
		}
	}
}
