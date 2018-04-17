
#pragma once

#include "asd.Core.Base.h"

namespace asd
{
	/**
		@brief	friend classを用いてprivateな関数にアクセスするためのクラス
	*/
	class Accessor
	{
		Accessor() {}
		~Accessor() {}
	public:

		static AnimationSource* AnimationClip_GetSource(AnimationClip* o);

		static Texture2D* Material2D_GetTexture2D(Material2D* o, const achar* name);
		static Shader2D* Material2D_GetShader2D(Material2D* o);

		static Texture2D* Material3D_GetTexture2D(Material3D* o, const achar* name);
		static Shader3D* Material3D_GetShader3D(Material3D* o);

		static Texture2D* MaterialPropertyBlock_GetTexture2D(MaterialPropertyBlock* o, const achar* name);

		static AnimationClip* AnimationSyatem_CreateAnimationClip(AnimationSystem* o);
		static AnimationSource* AnimationSyatem_CreateAnimationSource(AnimationSystem* o);
		static KeyframeAnimation* AnimationSyatem_CreateKeyframeAnimation(AnimationSystem* o);

		static AnimationClip* Model_GetAnimationClip(Model* o, int32_t index);
		static Mesh* Model_GetMesh(Model* o, int32_t index);

		static Texture2D* ImagePackage_GetImage(ImagePackage* o, int32_t index);

		static Effect* CoreEffectObject2D_GetEffect(CoreEffectObject2D* o);

		static Texture2D* Chip2D_GetTexture(CoreChip2D* chip);
	};
}