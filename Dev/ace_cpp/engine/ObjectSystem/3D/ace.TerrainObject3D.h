
#pragma once

#include "ace.Object3D.h"

namespace ace
{
	/**
	@brief	地形を描画するクラス
	*/
	class TerrainObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreTerrainObject3D> m_coreObject;

	protected:
		/**
		@brief	オーバーライドして、このオブジェクトの初期化処理を記述できる。
		*/
		virtual void OnStart() {};

		/**
		@brief	オーバーライドして、毎フレーム実行される更新処理を記述できる。
		*/
		virtual void OnUpdate() {};

		/**
		@brief	オーバーライドして、追加の描画処理を記述できる。
		*/
		virtual void OnDrawAdditionally() {};

	public:
		TerrainObject3D();
		virtual ~TerrainObject3D();

		/**
			@brief	地形を設定する。
			@param	terrain	地形
		*/
		void SetTerrain(std::shared_ptr<Terrain3D> terrain);
	};
}