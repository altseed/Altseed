
#pragma once

#include "ace.Object3D.h"

namespace ace
{
	/**
	@brief	大量にメッシュを表示する3Dオブジェクトの機能を提供するクラス
	*/
	class MassModelObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreMassModelObject3D> m_coreObject;

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
		MassModelObject3D();
		virtual ~MassModelObject3D();

		/**
		@brief	描画に使用するモデルを設定する。
		@param	model	モデル
		*/
		void SetModel(std::shared_ptr<MassModel>& model);

		/**
		@brief	アニメーションを再生する。
		@param	name	アニメーションの名称
		*/
		void PlayAnimation(const achar* name);

		/**
		@brief	アニメーションを停止する。
		*/
		void StopAnimation();

		/**
		@brief	アニメーションをクロスフェードで再生する。
		@param	name	アニメーションの名称
		@param	time	クロスフェードにかかる時間(秒)
		*/
		void CrossFadeAnimation(const achar* name, float time);

		/**
		@brief	アニメーションが再生中か取得する。
		@return	再生中か?
		*/
		bool IsAnimationPlaying();
	};
}