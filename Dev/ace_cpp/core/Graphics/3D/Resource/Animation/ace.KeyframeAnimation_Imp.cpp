
#include "ace.KeyframeAnimation_Imp.h"

#include <cmath>

namespace ace
{
	KeyframeAnimation_Imp::KeyframeAnimation_Imp()
		: m_targetType(AnimationCurveTargetType::None)
		, m_targetAxis(AnimationCurveTargetAxis::None)
	{
	
	}

	KeyframeAnimation_Imp::~KeyframeAnimation_Imp()
	{
	
	}

	const achar* KeyframeAnimation_Imp::GetName()
	{
		return m_name.c_str();
	}

	void KeyframeAnimation_Imp::SetName(const achar* name)
	{
		m_name = name;

		ModelUtils::GetAnimationTarget(m_targetName, m_targetType, m_targetAxis, name);
	}

	astring& KeyframeAnimation_Imp::GetTargetName()
	{
		return m_targetName;
	}

	AnimationCurveTargetType KeyframeAnimation_Imp::GetTargetType()
	{
		return m_targetType;
	}

	AnimationCurveTargetAxis KeyframeAnimation_Imp::GetTargetAxis()
	{
		return m_targetAxis;
	}

	void KeyframeAnimation_Imp::AddKeyframe(const FCurveKeyframe& kf)
	{
		m_keyframes.push_back(kf);
	}

	float KeyframeAnimation_Imp::GetValue(float time)
	{
		return ModelUtils::GetKeyframeValue(time, m_keyframes);
	}
}
