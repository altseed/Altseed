
#pragma once

#include "ace.Object3D.h"

namespace ace
{
	/**
	@brief	エフェクトの機能を提供するクラス
	*/
	class EffectObject3D
		: public Object3D
	{
	private:
		std::shared_ptr<CoreEffectObject3D> m_coreObject;

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
		EffectObject3D();
		virtual ~EffectObject3D();

		/**
			@brief	描画に使用するエフェクトを設定する。
			@param	effect	エフェクト
		*/
		void SetEffect(std::shared_ptr<Effect> effect);


		/**
		@brief	設定されている全てのエフェクトを再生する。
		*/
		void Play();

		/**
		@brief	このオブジェクトから再生されたエフェクトを全て停止する。
		*/
		void Stop();

		/**
		@brief	このオブジェクトから再生されたエフェクトのルートを全て停止する。
		*/
		void StopRoot();

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか取得する。
		@return	フラグ
		*/
		bool GetSyncEffects();

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか設定する。
		@param	value	フラグ
		*/
		void SetSyncEffects(bool value);
	};
}