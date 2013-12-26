
#pragma once

#include "../../../ace.Core.Base_Imp.h"
#include "ace.KeyframeAnimation.h"

namespace ace
{
	class KeyframeAnimation_Imp
	{
	private:

		std::vector<Keyframe>	m_keyframes;

	public:
		KeyframeAnimation_Imp();
		virtual ~KeyframeAnimation_Imp();

		void AddKeyframe(Keyframe kf);

		float GetValue(float frame);
	};
}