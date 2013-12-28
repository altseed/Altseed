
#pragma once

#include "../../../ace.Core.Base_Imp.h"
#include "ace.KeyframeAnimation.h"

namespace ace
{
	enum eAnimationCurveTargetType
	{
		ANIMATION_CURVE_TARGET_TYPE_NONE,
		ANIMATION_CURVE_TARGET_TYPE_POSITON,
		ANIMATION_CURVE_TARGET_TYPE_ROTATION,
		ANIMATION_CURVE_TARGET_TYPE_SCALE,
	};

	enum eAnimationCurveTargetAxis
	{
		ANIMATION_CURVE_TARGET_AXIS_NONE,
		ANIMATION_CURVE_TARGET_AXIS_X,
		ANIMATION_CURVE_TARGET_AXIS_Y,
		ANIMATION_CURVE_TARGET_AXIS_Z,
		ANIMATION_CURVE_TARGET_AXIS_W,
	};

	
	class KeyframeAnimation_Imp
		: public KeyframeAnimation
		, public ReferenceObject
	{
	private:

		astring					m_name;
		std::vector<Keyframe>	m_keyframes;

		astring						m_targetName;
		eAnimationCurveTargetType	m_targetType;
		eAnimationCurveTargetAxis	m_targetAxis;

	public:
		KeyframeAnimation_Imp();
		virtual ~KeyframeAnimation_Imp();

		const achar* GetName();

		void SetName(const achar* name);

		const achar* GetTargetName();

		eAnimationCurveTargetType GetTargetType();

		eAnimationCurveTargetAxis GetTargetAxis();

		void AddKeyframe(Keyframe kf) override;

		float GetValue(float frame) override;

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}