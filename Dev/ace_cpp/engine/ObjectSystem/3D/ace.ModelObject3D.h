
#pragma once

#include "ace.Object3D.h"

namespace ace
{
	/**
	@brief	メッシュを表示する3Dオブジェクトの機能を提供するクラス
	*/
	class ModelObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreModelObject3D> m_coreObject;

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
		ModelObject3D();
		virtual ~ModelObject3D();

		/**
		@brief	描画に使用するモデルを設定する。
		@param	model	モデル
		*/
		void SetModel(std::shared_ptr<Model>& model);

		/**
		@brief	描画に使用するメッシュを追加する。
		@param	meshGroupIndex	メッシュグループのインデックス
		@param	mesh	メッシュ
		*/
		void AddMesh(std::shared_ptr<Mesh> mesh);

		/**
		@brief	描画に使用するデフォーマーを設定する。
		@param	meshGroupIndex	メッシュグループのインデックス
		@param	deformer	デフォーマー
		*/
		void SetDeformer(std::shared_ptr<Deformer> deformer);

		/**
		@brief	アニメーションを再生する。
		@param	name	アニメーションの名称
		*/
		void PlayAnimation(const achar* name);
	};
}