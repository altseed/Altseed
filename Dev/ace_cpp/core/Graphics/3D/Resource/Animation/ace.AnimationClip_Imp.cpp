
#include "ace.AnimationClip_Imp.h"
#include "ace.AnimationSource_Imp.h"

namespace ace
{
	AnimationClip_Imp::AnimationClip_Imp()
		: m_source(nullptr)
	{

	}

	AnimationClip_Imp::~AnimationClip_Imp()
	{
		SafeRelease(m_source);
	}

	AnimationSource* AnimationClip_Imp::GetSource_()
	{
		return m_source;
	}

	void AnimationClip_Imp::SetSource(AnimationSource* source)
	{
		SafeSubstitute(m_source, source);
	}

	bool AnimationClip_Imp::GetIsLoopingMode() const
	{
		return isLoopingMode;
	}

	void AnimationClip_Imp::SetIsLoopingMode(bool isLoopingMode)
	{
		this->isLoopingMode = isLoopingMode;
	}

}