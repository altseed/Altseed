
#pragma once

#include "../../../ace.Core.Base.h"
#include "../../../ace.ReferenceObject.h"

namespace ace
{
	class AnimationSource
		: public IReference
	{
	public:
		AnimationSource() {}
		virtual ~AnimationSource() {}

		virtual void AddAnimation(KeyframeAnimation* keyframeAnimation) = 0;
		virtual void RemoveAnimation(KeyframeAnimation* keyframeAnimation) = 0;

#if !SWIG
		void AddAnimation(std::shared_ptr<KeyframeAnimation> keyframeAnimation)
		{
			AddAnimation(keyframeAnimation.get());
		}

		void RemoveAnimation(std::shared_ptr<KeyframeAnimation> keyframeAnimation)
		{
			RemoveAnimation(keyframeAnimation.get());
		}
#endif
	};
}

