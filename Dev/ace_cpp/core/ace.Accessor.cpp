
#include "ace.Accessor.h"

#include "Sound/ace.Sound_Imp.h"

#include "Graphics/3D/Resource/Animation/ace.AnimationSystem.h"
#include "Graphics/3D/Resource/Animation/ace.AnimationClip.h"
#include "Graphics/3D/Resource/Animation/ace.AnimationSource.h"

#include "Graphics/Resource/ace.Shader2D.h"
#include "Graphics/Resource/ace.Material2D.h"

#include "Graphics/Resource/ace.Shader3D.h"
#include "Graphics/Resource/ace.Material3D.h"

#include "Graphics/Resource/ace.MaterialPropertyBlock.h"

#include "Graphics/Resource/ace.ImagePackage.h"

#include "ObjectSystem/2D/ace.CoreEffectObject2D.h"

#include "Graphics/3D/Resource/ace.Mesh.h"
#include "Graphics/3D//Resource/ace.Model.h"

#include "Graphics/Resource/ace.Chip2D.h"

#include "IO/ace.StreamFile.h"

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

	Texture2D* Accessor::Material3D_GetTexture2D(Material3D* o, const achar* name)
	{
		return o->GetTexture2D_(name);
	}

	Shader3D* Accessor::Material3D_GetShader3D(Material3D* o)
	{
		return o->GetShader3D_();
	}

	Texture2D* Accessor::MaterialPropertyBlock_GetTexture2D(MaterialPropertyBlock* o, const achar* name)
	{
		return o->GetTexture2D_(name);
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

	AnimationClip* Accessor::Model_GetAnimationClip(Model* o, int32_t index)
	{
		return o->GetAnimationClip_(index);
	}

	Mesh* Accessor::Model_GetMesh(Model* o, int32_t index)
	{
		return o->GetMesh_(index);
	}

	Texture2D* Accessor::ImagePackage_GetImage(ImagePackage* o, int32_t index)
	{
		return o->GetImage_(index);
	}

	Effect* Accessor::CoreEffectObject2D_GetEffect(CoreEffectObject2D* o)
	{
		return o->GetEffect_();
	}

	Texture2D* Accessor::Chip2D_GetTexture(Chip2D* chip)
	{
		return chip->GetTexture_();
	}


	int32_t Accessor::StreamFile_Read_(StreamFile* o, int32_t size)
	{
		return o->Read_(size);
	}

	void* Accessor::StreamFile_GetTempBuffer_(StreamFile* o)
	{
		return o->GetTempBuffer_();
	}

	int32_t Accessor::StreamFile_GetTempBufferSize_(StreamFile* o)
	{
		return o->GetTempBufferSize_();
	}
}