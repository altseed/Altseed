
#include "ace.AnimationSystem_Imp.h"

#include "ace.AnimationClip_Imp.h"
#include "ace.AnimationSource_Imp.h"
#include "ace.KeyframeAnimation_Imp.h"

namespace ace
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