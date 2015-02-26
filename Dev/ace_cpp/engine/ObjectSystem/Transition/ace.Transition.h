
#pragma once
#include "../../ace.CoreToEngine.h"

namespace ace
{
	/**
		@brief	シーン遷移時の効果を適用するクラス
	*/
	class Transition
	{
		friend class Engine;

	private:
		std::shared_ptr<CoreTransition> coreTransition;

		std::shared_ptr<CoreTransition> GetCoreTransition() { return coreTransition; }

	public:
		Transition();
		virtual ~Transition();

		/**
		@brief	オーバーライドして、このオブジェクトの更新処理を記述できる。
		*/
		virtual void OnUpdate();


		/**
		@brief	シーンが切り替わっているか取得する。
		@return	シーンが切り替わっているか?
		*/
		bool IsSceneChanged() const;

		/**
		@brief	実際にシーンを次のシーンに切り替える。
		*/
		void ChangeScene();

		/**
		@brief	シーン遷移処理が終了したか取得する。
		@return	シーン遷移処理が終了したか?
		*/
		bool GetIsFinished() const;

		/**
		@brief	シーン遷移処理を終了する。
		*/
		void Finish();

		/**
		@brief	次のシーンを3角形に貼り付けて描画する。
		@param	pos1	座標1(0～1)
		@param	pos2	座標2(0～1)
		@param	pos3	座標3(0～1)
		@param	col1	頂点カラー1
		@param	col2	頂点カラー2
		@param	col3	頂点カラー3
		@param	uv1		UV1(0～1)
		@param	uv2		UV2(0～1)
		@param	uv3		UV3(0～1)
		@note
		ChangeSceneを実行するまでは無効である。
		*/
		virtual void DrawTriangleWithNextScene(
			Vector2DF pos1, Vector2DF pos2, Vector2DF pos3,
			Color col1, Color col2, Color col3,
			Vector2DF uv1, Vector2DF uv2, Vector2DF uv3);

		/**
		@brief	次のシーンを4角形に貼り付けて描画する。
		@param	upperLeftPos	左上の描画位置(0～1)
		@param	upperRightPos	右上の描画位置(0～1)
		@param	lowerRightPos	右下の描画位置(0～1)
		@param	lowerLeftPos	左下の描画位置(0～1)
		@param	upperLeftCol	左上の頂点色
		@param	upperRightCol	右上の頂点色
		@param	lowerRightCol	右下の頂点色
		@param	lowerLeftCol	左下の頂点色
		@param	upperLeftUV		左上のUV値(0～1)
		@param	upperRightUV	右上のUV値(0～1)
		@param	lowerRightUV	右下のUV値(0～1)
		@param	lowerLeftUV		左下のUV値(0～1)
		@note
		ChangeSceneを実行するまでは無効である。
		*/
		void DrawRectangleWithNextScene(
			Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV);

		/**
		@brief	前のシーンを3角形に貼り付けて描画する。
		@param	pos1	座標1
		@param	pos2	座標2
		@param	pos3	座標3
		@param	col1	頂点カラー1
		@param	col2	頂点カラー2
		@param	col3	頂点カラー3
		@param	uv1		UV1
		@param	uv2		UV2
		@param	uv3		UV3
		@note
		ChangeSceneを実行すると前のシーンの描画は停止し、静止画として扱われる。
		*/
		void DrawTriangleWithPreviousScene(
			Vector2DF pos1, Vector2DF pos2, Vector2DF pos3,
			Color col1, Color col2, Color col3,
			Vector2DF uv1, Vector2DF uv2, Vector2DF uv3);

		/**
		@brief	前のシーンを4角形に貼り付けて描画する。
		@param	upperLeftPos	左上の描画位置(0～1)
		@param	upperRightPos	右上の描画位置(0～1)
		@param	lowerRightPos	右下の描画位置(0～1)
		@param	lowerLeftPos	左下の描画位置(0～1)
		@param	upperLeftCol	左上の頂点色
		@param	upperRightCol	右上の頂点色
		@param	lowerRightCol	右下の頂点色
		@param	lowerLeftCol	左下の頂点色
		@param	upperLeftUV		左上のUV値(0～1)
		@param	upperRightUV	右上のUV値(0～1)
		@param	lowerRightUV	右下のUV値(0～1)
		@param	lowerLeftUV		左下のUV値(0～1)
		@note
		ChangeSceneを実行すると前のシーンの描画は停止し、静止画として扱われる。
		*/
		void DrawRectangleWithPreviousScene(
			Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV);
	};
}