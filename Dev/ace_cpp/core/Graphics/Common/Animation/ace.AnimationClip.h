
#pragma once

#include "../../../ace.Core.Base.h"
#include "../../../ace.ReferenceObject.h"

namespace ace
{
	class AnimationClip
		: public IReference
	{
	public:

		AnimationClip() {}
		virtual ~AnimationClip() {}

		virtual AnimationSource* GetSource() = 0;
		virtual void SetSource(AnimationSource* source) = 0;
	};
}