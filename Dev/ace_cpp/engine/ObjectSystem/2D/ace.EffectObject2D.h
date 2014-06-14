#pragma once

#include "ace.Object2D.h"

namespace ace
{
	class EffectObject2D : public Object2D
	{
	private:
		std::shared_ptr<CoreEffectObject2D> m_coreObject;

		CoreObject2D* GetCoreObject() const;

	public:
		typedef std::shared_ptr<EffectObject2D> Ptr;

		EffectObject2D();
		virtual ~EffectObject2D();

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