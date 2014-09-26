
#include "ace.AnimationSource_Imp.h"
#include "ace.KeyframeAnimation_Imp.h"

namespace ace
{
	AnimationSource_Imp::AnimationSource_Imp()
	{
	
	}

	AnimationSource_Imp::~AnimationSource_Imp()
	{
		for (auto& a : m_animations)
		{
			a->Release();
		}
		m_animations.clear();
	}

	void AnimationSource_Imp::AddAnimation(KeyframeAnimation* keyframeAnimation)
	{
		if (m_animations.find(keyframeAnimation) == m_animations.end())
		{
			SafeAddRef(keyframeAnimation);
			m_animations.insert(keyframeAnimation);

			auto kf = (KeyframeAnimation_Imp*) keyframeAnimation;
			if (length < kf->GetLength())
			{
				length = kf->GetLength();
			}
		}
	}

	void AnimationSource_Imp::RemoveAnimation(KeyframeAnimation* keyframeAnimation)
	{
		if (m_animations.find(keyframeAnimation) != m_animations.end())
		{
			m_animations.erase(keyframeAnimation);

			auto kf = (KeyframeAnimation_Imp*) keyframeAnimation;
			if (length == kf->GetLength())
			{
				for (auto& a : m_animations)
				{
					auto kf_ = (KeyframeAnimation_Imp*) a;
					if (length < kf_->GetLength())
					{
						length = kf_->GetLength();
					}
				}
			}

			SafeRelease(keyframeAnimation);
		}
	}
}