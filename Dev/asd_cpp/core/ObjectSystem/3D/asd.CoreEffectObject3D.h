
#pragma once

#include "asd.CoreObject3D.h"

#include <Graphics/asd.Color.h>

namespace asd
{
	class CoreEffectObject3D
		: public CoreObject3D
	{
	protected:
		CoreEffectObject3D(){}
		virtual ~CoreEffectObject3D(){}

	public:

		virtual void SetEffect(Effect* effect) = 0;

		virtual int32_t Play() = 0;

		virtual void Stop() = 0;

		virtual void StopRoot() = 0;

		virtual void Show() = 0;

		virtual void Hide() = 0;

		/**
		@brief	このオブジェクトから再生されたエフェクトが再生中か取得する。
		*/
		virtual bool GetIsPlaying() = 0;

		virtual bool GetSyncEffects() = 0;

		virtual void SetSyncEffects(bool value) = 0;
	};
}
