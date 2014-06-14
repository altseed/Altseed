
#pragma once
#include "ace.CoreObject2D.h"

namespace ace
{
	class CoreEffectObject2D
		: public CoreObject2D
	{
	public:
		CoreEffectObject2D() {}
		virtual ~CoreEffectObject2D() {}

		virtual void SetEffect(Effect* effect) = 0;

		/**
		@brief	設定されている全てのエフェクトを再生する。
		*/
		virtual void Play() = 0;

		/**
		@brief	このオブジェクトから再生されたエフェクトを全て停止する。
		*/
		virtual void Stop() = 0;

		/**
		@brief	このオブジェクトから再生されたエフェクトのルートを全て停止する。
		*/
		virtual void StopRoot() = 0;

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか取得する。
		@return	フラグ
		*/
		virtual bool GetSyncEffects() = 0;

		/**
		@brief	このオブジェクトから再生されたエフェクトをオブジェクトに合わせて移動させるか設定する。
		@param	value	フラグ
		*/
		virtual void SetSyncEffects(bool value) = 0;

	};
}