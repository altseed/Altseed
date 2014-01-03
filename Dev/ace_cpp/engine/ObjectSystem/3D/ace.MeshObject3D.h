
#pragma once

#include "ace.Object3D.h"

namespace ace
{
	/**
	@brief	メッシュを表示する3Dオブジェクトの機能を提供するクラス
	*/
	class MeshObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreMeshObject3D> m_coreObject;

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
		MeshObject3D();
		virtual ~MeshObject3D();

		/**
			@brief	描画に使用するメッシュを設定する。
			@param	mesh	メッシュ
		*/
		void AddMesh(std::shared_ptr<Mesh>& mesh);
	};
}