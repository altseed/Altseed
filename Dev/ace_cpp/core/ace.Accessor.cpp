
#include "ace.Accessor.h"

#include "Sound/ace.Sound_Imp.h"

#include "Graphics/3D/Resource/Animation/ace.AnimationSystem.h"
#include "Graphics/3D/Resource/Animation/ace.AnimationClip.h"
#include "Graphics/3D/Resource/Animation/ace.AnimationSource.h"

#include "Graphics/Resource/ace.Shader2D.h"
#include "Graphics/Resource/ace.Material2D.h"

#include "ObjectSystem/2D/ace.CoreEffectObject2D.h"

#include "Graphics/3D/Resource/ace.Mesh.h"
#include "Graphics/3D//Resource/ace.Model.h"

namespace ace
{
	SoundSource* Accessor::Sound_CreateSoundSource(Sound* o, const achar* path, bool isDecompressed)
	{
		return o->CreateSoundSource_(path, isDecompressed);
	}

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

	AnimationClip* Accessor::AnimationSyatem_CreateAnimationClip(AnimationSystem* o)
	{
		return o->CreateAnimationClip_();
	}

	AnimationSource* Accessor::AnimationSyatem_CreateAnimationSource(AnimationSystem* o)
	{
		return o->CreateAnimationSource_();
	}

	KeyframeAnimation* Accessor::AnimationSyatem_CreateKeyframeAnimation(AnimationSystem* o)
	{
		return o->CreateKeyframeAnimation_();
	}

	Mesh* Accessor::Model_GetMesh(Model* o, int32_t index)
	{
		return o->GetMesh_(index);
	}

	Effect* Accessor::CoreEffectObject2D_GetEffect(CoreEffectObject2D* o)
	{
		return o->GetEffect_();
	}
}