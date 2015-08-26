using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// シーン遷移時の効果を適用するクラス
	/// </summary>
	public class Transition : IReleasable
	{
		internal swig.CoreTransition SwigObject;

		public Transition()
		{
			SwigObject = Engine.ObjectSystemFactory.CreateTransition();

			var p = SwigObject.GetPtr();
			if (GC.Transitions.GetObject(p) != null)
			{
				Particular.Helper.ThrowException("");
			}
			GC.Transitions.AddObject(p, this);
		}

		#region GC対応
		~Transition()
		{
			ForceToRelease();
		}

		public bool IsReleased
		{
			get { return SwigObject == null; }
		}

		/// <summary>
		/// 強制的に使用しているメモリを開放する。
		/// </summary>
		/// <remarks>
		/// 何らかの理由でメモリが不足した場合に実行する。
		/// 開放した後の動作の保証はしていないので、必ず参照が残っていないことを確認する必要がある。
		/// </remarks>
		public void ForceToRelease()
		{
			lock (this)
			{
				if (SwigObject == null) return;
				GC.Collector.AddObject(SwigObject);
				SwigObject = null;
			}
			Particular.GC.SuppressFinalize(this);
		}
		#endregion

		/// <summary>
		/// オーバーライドして、このオブジェクトの更新処理を記述できる。
		/// </summary>
		internal protected virtual void OnUpdate() { }

		/// <summary>
		/// シーンが切り替わっているか取得する。
		/// </summary>
		public bool IsSceneChanged
		{
			get
			{
				return SwigObject.GetIsSceneChanged();
			}
		}

		/// <summary>
		/// 実際にシーンを次のシーンに切り替える。
		/// </summary>
		public void ChangeScene()
		{
			SwigObject.ChangeScene();
		}

		/// <summary>
		/// シーン遷移処理が終了したか取得する。
		/// </summary>
		public bool IsFinished
		{
			get
			{
				return SwigObject.GetIsFinished();
			}
		}

		/// <summary>
		/// シーン遷移処理を終了する。
		/// </summary>
		public void Finish()
		{
			SwigObject.Finish();
		}

		/// <summary>
		/// 次のシーンを3角形に貼り付けて描画する。
		/// </summary>
		/// <param name="pos1">座標1(0～1)</param>
		/// <param name="pos2">座標2(0～1)</param>
		/// <param name="pos3">座標3(0～1)</param>
		/// <param name="col1">頂点カラー1</param>
		/// <param name="col2">頂点カラー2</param>
		/// <param name="col3">頂点カラー3</param>
		/// <param name="uv1">UV1(0～1)</param>
		/// <param name="uv2">UV2(0～1)</param>
		/// <param name="uv3">UV3(0～1)</param>
		/// <remarks>ChangeSceneを実行するまでは無効である。</remarks>
		public void DrawTriangleWithNextScene(
		Vector2DF pos1, Vector2DF pos2, Vector2DF pos3,
		Color col1, Color col2, Color col3,
		Vector2DF uv1, Vector2DF uv2, Vector2DF uv3)
		{
			SwigObject.DrawTriangleWithNextScene(
				pos1, pos2, pos3,
				col1, col2, col3,
				uv1, uv2, uv3);
		}

		/// <summary>
		/// 次のシーンを4角形に貼り付けて描画する。
		/// </summary>
		/// <param name="upperLeftPos">左上の描画位置(0～1)</param>
		/// <param name="upperRightPos">右上の描画位置(0～1)</param>
		/// <param name="lowerRightPos">右下の描画位置(0～1)</param>
		/// <param name="lowerLeftPos">左下の描画位置(0～1)</param>
		/// <param name="upperLeftCol">左上の頂点色</param>
		/// <param name="upperRightCol">右上の頂点色</param>
		/// <param name="lowerRightCol">右下の頂点色</param>
		/// <param name="lowerLeftCol">左下の頂点色</param>
		/// <param name="upperLeftUV">左上のUV値(0～1)</param>
		/// <param name="upperRightUV">右上のUV値(0～1)</param>
		/// <param name="lowerRightUV">右下のUV値(0～1)</param>
		/// <param name="lowerLeftUV">左下のUV値(0～1)</param>
		/// <remarks>ChangeSceneを実行するまでは無効である。</remarks>
		public void DrawRectangleWithNextScene(
			Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV)
		{
			SwigObject.DrawRectangleWithNextScene(
				upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos,
				upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol,
				upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV);
		}

		/// <summary>
		/// 前のシーンを3角形に貼り付けて描画する。
		/// </summary>
		/// <param name="pos1">座標1(0～1)</param>
		/// <param name="pos2">座標2(0～1)</param>
		/// <param name="pos3">座標3(0～1)</param>
		/// <param name="col1">頂点カラー1</param>
		/// <param name="col2">頂点カラー2</param>
		/// <param name="col3">頂点カラー3</param>
		/// <param name="uv1">UV1(0～1)</param>
		/// <param name="uv2">UV2(0～1)</param>
		/// <param name="uv3">UV3(0～1)</param>
		/// <remarks>ChangeSceneを実行すると前のシーンの描画は停止し、静止画として扱われる。</remarks>
		public void DrawTriangleWithPreviousScene(
		Vector2DF pos1, Vector2DF pos2, Vector2DF pos3,
		Color col1, Color col2, Color col3,
		Vector2DF uv1, Vector2DF uv2, Vector2DF uv3)
		{
			SwigObject.DrawTriangleWithPreviousScene(
				pos1, pos2, pos3,
				col1, col2, col3,
				uv1, uv2, uv3);
		}

		/// <summary>
		/// 前のシーンを4角形に貼り付けて描画する。
		/// </summary>
		/// <param name="upperLeftPos">左上の描画位置(0～1)</param>
		/// <param name="upperRightPos">右上の描画位置(0～1)</param>
		/// <param name="lowerRightPos">右下の描画位置(0～1)</param>
		/// <param name="lowerLeftPos">左下の描画位置(0～1)</param>
		/// <param name="upperLeftCol">左上の頂点色</param>
		/// <param name="upperRightCol">右上の頂点色</param>
		/// <param name="lowerRightCol">右下の頂点色</param>
		/// <param name="lowerLeftCol">左下の頂点色</param>
		/// <param name="upperLeftUV">左上のUV値(0～1)</param>
		/// <param name="upperRightUV">右上のUV値(0～1)</param>
		/// <param name="lowerRightUV">右下のUV値(0～1)</param>
		/// <param name="lowerLeftUV">左下のUV値(0～1)</param>
		/// <remarks>ChangeSceneを実行すると前のシーンの描画は停止し、静止画として扱われる。</remarks>
		public void DrawRectangleWithPreviousScene(
			Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV)
		{
			SwigObject.DrawRectangleWithPreviousScene(
				upperLeftPos, upperRightPos, lowerRightPos, lowerLeftPos,
				upperLeftCol, upperRightCol, lowerRightCol, lowerLeftCol,
				upperLeftUV, upperRightUV, lowerRightUV, lowerLeftUV);
		}
	}
}
