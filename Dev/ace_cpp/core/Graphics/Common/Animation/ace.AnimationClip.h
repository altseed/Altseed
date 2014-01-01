
#pragma once

#include "../../../ace.Core.Base.h"
#include "../../../ace.ReferenceObject.h"

namespace ace
{
	class AnimationClip
		: public IReference
	{
	protected:
		virtual AnimationSource* GetSource_() = 0;
	public:

		AnimationClip() {}
		virtual ~AnimationClip() {}

		virtual void SetSource(AnimationSource* source) = 0;

#if !SWIG
		std::shared_ptr<AnimationSource> GetSource()
		{
			auto o = GetSource_();
			SafeAddRef(o);
			return CreateSharedPtrWithReleaseDLL(o);
		}

		void SetSource(std::shared_ptr<AnimationSource>& source)
		{
			SetSource(source);
		}
#endif
	};
}