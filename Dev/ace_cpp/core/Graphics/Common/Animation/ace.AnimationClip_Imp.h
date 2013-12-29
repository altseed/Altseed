
#pragma once

#include "../../../ace.Core.Base_Imp.h"
#include "ace.AnimationClip.h"

namespace ace
{
	class AnimationClip_Imp
		: public AnimationClip
		, public ReferenceObject
	{
	private:
		AnimationSource*	m_source;

	public:

		AnimationClip_Imp();
		virtual ~AnimationClip_Imp();

		AnimationSource* GetSource();
		void SetSource(AnimationSource* source);
	};
}