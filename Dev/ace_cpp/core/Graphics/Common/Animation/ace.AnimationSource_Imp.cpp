
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
		}
	}

	void AnimationSource_Imp::RemoveAnimation(KeyframeAnimation* keyframeAnimation)
	{
		if (m_animations.find(keyframeAnimation) != m_animations.end())
		{
			SafeRelease(keyframeAnimation);
			m_animations.erase(keyframeAnimation);
		}
	}
}