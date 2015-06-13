
#include "asd.AnimationSystem_Imp.h"

#include "asd.AnimationClip_Imp.h"
#include "asd.AnimationSource_Imp.h"
#include "asd.KeyframeAnimation_Imp.h"

namespace asd
{
	AnimationSystem_Imp::AnimationSystem_Imp()
	{

	}

	AnimationSystem_Imp::~AnimationSystem_Imp()
	{

	}

	AnimationClip* AnimationSystem_Imp::CreateAnimationClip_()
	{
		return new AnimationClip_Imp();
	}

	AnimationSource* AnimationSystem_Imp::CreateAnimationSource_()
	{
		return new AnimationSource_Imp();
	}

	KeyframeAnimation* AnimationSystem_Imp::CreateKeyframeAnimation_()
	{
		return new KeyframeAnimation_Imp();
	}

}