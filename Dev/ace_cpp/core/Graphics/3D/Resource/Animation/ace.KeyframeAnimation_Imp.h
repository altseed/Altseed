
#pragma once

#include "../../../../ace.Core.Base_Imp.h"
#include "ace.KeyframeAnimation.h"

#include <Graphics/3D/ace.Model_IO.h>

namespace ace
{
	class KeyframeAnimation_Imp
		: public KeyframeAnimation
		, public ReferenceObject
	{
	private:

		astring					m_name;
		std::vector<FCurveKeyframe>	m_keyframes;

		astring						m_targetName;
		AnimationCurveTargetType	m_targetType;
		AnimationCurveTargetAxis	m_targetAxis;

	public:
		KeyframeAnimation_Imp();
		virtual ~KeyframeAnimation_Imp();

		const achar* GetName();

		void SetName(const achar* name);

		astring& GetTargetName();

		AnimationCurveTargetType GetTargetType();

		AnimationCurveTargetAxis GetTargetAxis();

		void AddKeyframe(const FCurveKeyframe& kf) override;

		float GetValue(float time) override;

		float GetLength()
		{
			if (m_keyframes.size() == 0) return 0;
			return (*(m_keyframes.end() - 1)).KeyValue.X;
		}

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}