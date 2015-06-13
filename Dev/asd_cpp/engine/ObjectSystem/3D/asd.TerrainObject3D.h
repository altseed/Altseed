
#pragma once

#include "asd.Object3D.h"

namespace asd
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

		/**
		@brief	材質個別に描画時のパラメーターを設定する。
		@param	block	プロパティ
		@note
		通常描画する場合、材質に設定されたパラメーターを元に描画するがモデルごとに個別のパラメーターを使用する場合、この関数を使用する。
		*/
		void SetMaterialPropertyBlock(std::shared_ptr<MaterialPropertyBlock> block);
	};
}