
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
		@brief	アニメーションクリップを取得する。
		@param	name	アニメーションクリップの名称
		@return	アニメーションクリップ
		*/
		std::shared_ptr<AnimationClip> GetAnimationClip(const achar* name);

		/**
		@brief	アニメーションを再生する。
		@param	index	対象のインデックス
		@param	name	アニメーションの名称
		*/
		void PlayAnimation(int32_t index, const achar* name);

		/**
		@brief	アニメーションを停止する。
		@param	index	対象のインデックス
		*/
		void StopAnimation(int32_t index);

		/**
		@brief	アニメーションの合成比率を設定する。
		@param	index	対象のインデックス
		@param	weight	合成比率
		*/
		void SetAnimationWeight(int32_t index, float weight);

		/**
		@brief	アニメーションをクロスフェードで再生する。
		@param	index	対象のインデックス
		@param	name	アニメーションの名称
		@param	time	クロスフェードにかかる時間(秒)
		*/
		void CrossFadeAnimation(int32_t index, const achar* name, float time);

		/**
		@brief	アニメーションが再生中か取得する。
		@param	index	対象のインデックス
		@return	再生中か?
		*/
		bool IsAnimationPlaying(int32_t index);

		/**
		@brief	材質個別に描画時のパラメーターを設定する。
		@param	meshIndex	メッシュのインデックス
		@param	materialIndex	材質のインデックス
		@param	block	プロパティ
		@note
		通常描画する場合、材質に設定されたパラメーターを元に描画するがモデルごとに個別のパラメーターを使用する場合、この関数を使用する。
		*/
		void SetMaterialPropertyBlock(int32_t meshIndex, int32_t materialIndex, std::shared_ptr<MaterialPropertyBlock> block);
	};
}