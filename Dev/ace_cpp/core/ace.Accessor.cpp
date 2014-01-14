
#include "ace.Accessor.h"

#include "Graphics/Common/Animation/ace.AnimationClip.h"
#include "Graphics/Common/Animation/ace.AnimationSource.h"

#include "Graphics/Common/Resource/ace.Shader2D.h"
#include "Graphics/Common/Resource/ace.Material2D.h"

namespace ace
{
	AnimationSource* Accessor::AnimationClip_GetSource(AnimationClip* o)
	{
		return o->GetSource_();
	}

	Texture2D* Accessor::Material2D_GetTexture2D(Material2D* o, const achar* name)
	{
		return o->GetTexture2D_(name);
	}

	Shader2D* Accessor::Material2D_GetShader2D(Material2D* o)
	{
		return o->GetShader2D_();
	}
}