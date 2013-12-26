
#pragma once

#include "../../../ace.Core.Base.h"

#include <Math/ace.Vector2DF.h>

namespace ace
{
	class Keyframe
	{
	public:
		Vector2DF	KeyValue;
		Vector2DF	LeftHandle;
		Vector2DF	RightHandle;
	};

	class KeyframeAnimation
	{
	private:

	public:
		KeyframeAnimation() {}
		virtual ~KeyframeAnimation() {}

		virtual void AddKeyframe(Keyframe kf) = 0;
	};
}