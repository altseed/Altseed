
#pragma once

#include "ace.CoreObject3D.h"

#include <Graphics/ace.Color.h>

namespace ace
{
	class CoreEffectObject3D
		: public CoreObject3D
	{
	protected:
		CoreEffectObject3D(){}
		virtual ~CoreEffectObject3D(){}

	public:

		virtual void SetEffect(Effect* effect) = 0;

		virtual void Play() = 0;

		virtual void Stop() = 0;

		virtual void StopRoot() = 0;

		/**
		@brief	このオブジェクトから再生されたエフェクトが再生中か取得する。
		*/
		virtual bool IsPlaying() = 0;

		virtual bool GetSyncEffects() = 0;

		virtual void SetSyncEffects(bool value) = 0;
	};
}
