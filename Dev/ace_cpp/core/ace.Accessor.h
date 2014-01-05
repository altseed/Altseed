
#pragma once

#include "ace.Core.Base.h"

namespace ace
{
	/**
		@brief	friend classを用いてprivateな関数にアクセスするためのクラス
	*/
	class Accessor
	{
		Accessor() {}
		~Accessor() {}
	public:
		static AnimationSource* AnimationClip_GetSource(AnimationClip* o);

	};
}