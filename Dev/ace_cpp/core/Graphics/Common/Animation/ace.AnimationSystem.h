
#pragma once

#include "../../../ace.Core.Base.h"
#include "../../../ace.ReferenceObject.h"

namespace ace
{
	class AnimationSystem
	{
	protected:
		AnimationSystem(){}
		virtual ~AnimationSystem(){}

		virtual AnimationClip* CreateAnimationClip_() = 0;

		virtual AnimationSource* CreateAnimationSource_() = 0;

		virtual KeyframeAnimation* CreateKeyframeAnimation_() = 0;

	public:

#if !SWIG
		std::shared_ptr<AnimationClip> CreateAnimationClip()
		{
			auto o = CreateAnimationClip_();
			return CreateSharedPtrWithReleaseDLL(o);
		}

		std::shared_ptr<AnimationSource> CreateAnimationSource()
		{
			auto o = CreateAnimationSource_();
			return CreateSharedPtrWithReleaseDLL(o);
		}

		std::shared_ptr<KeyframeAnimation> CreateKeyframeAnimation()
		{
			auto o = CreateKeyframeAnimation_();
			return CreateSharedPtrWithReleaseDLL(o);
		}
#endif
	};
}