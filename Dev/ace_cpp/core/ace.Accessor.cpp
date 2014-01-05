
#include "ace.Accessor.h"

#include "Graphics/Common/Animation/ace.AnimationClip.h"
#include "Graphics/Common/Animation/ace.AnimationSource.h"

namespace ace
{
	AnimationSource* Accessor::AnimationClip_GetSource(AnimationClip* o)
	{
		return o->GetSource_();
	}
}