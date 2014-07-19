
#pragma once

#include "../../ace.ReferenceObject.h"
#include <Math/ace.Vector2DF.h>
#include <Graphics/ace.Color.h>

namespace ace
{
	class CoreTransition
		: public IReference
	{
	private:

	public:
		CoreTransition() {}
		virtual ~CoreTransition() {}

		/**
			@brief	実際にシーンを次のシーンに切り替える。
		*/
		virtual void ChangeScene() = 0;

		/**
		@brief	次の画面を3角形に貼り付けて描画する。
			@param	pos1	座標1
			@param	uv1		UV1
			@param	uv1		頂点カラー1
			@param	pos2	座標2
			@param	uv2		UV2
			@param	uv2		頂点カラー2
			@param	pos3	座標3
			@param	uv3		UV3
			@param	uv3		頂点カラー3
			@note
			ChangeSceneを実行するまでは無効である。
		*/
		virtual void DrawNextScene(
			Vector2DF pos1, Vector2DF uv1, Color col1,
			Vector2DF pos2, Vector2DF uv2, Color col2,
			Vector2DF pos3, Vector2DF uv3, Color col3) = 0;
		
		/**
			@brief	前の画面を3角形に貼り付けて描画する。
			@param	pos1	座標1
			@param	uv1		UV1
			@param	uv1		頂点カラー1
			@param	pos2	座標2
			@param	uv2		UV2
			@param	uv2		頂点カラー2
			@param	pos3	座標3
			@param	uv3		UV3
			@param	uv3		頂点カラー3
			@note
			ChangeSceneを実行すると前のシーンの描画は停止する。
		*/
		virtual void DrawPreviousScene(
			Vector2DF pos1, Vector2DF uv1, Color col1,
			Vector2DF pos2, Vector2DF uv2, Color col2,
			Vector2DF pos3, Vector2DF uv3, Color col3) = 0;
	};
}