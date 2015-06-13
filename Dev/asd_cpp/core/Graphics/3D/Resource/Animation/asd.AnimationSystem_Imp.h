
#pragma once

#include "asd.AnimationSystem.h"

namespace asd
{
	class AnimationSystem_Imp
		: public AnimationSystem
	{
	private:
	public:
		AnimationSystem_Imp();
		virtual ~AnimationSystem_Imp();

		AnimationClip* CreateAnimationClip_() override;

		AnimationSource* CreateAnimationSource_() override;

		KeyframeAnimation* CreateKeyframeAnimation_() override;

	};
}