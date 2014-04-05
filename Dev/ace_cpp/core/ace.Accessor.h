
#pragma once

#include "ace.Core.Base.h"

namespace ace
{
	/**
		@brief	friend classを用いてprivateな関数にアクセスするためのクラス
	*/
	class Accessor
	{
		Accessor() {}
		~Accessor() {}
	public:

		static SoundSource* Sound_CreateSoundSource(Sound* o, const achar* path, bool isDecompressed);

		static AnimationSource* AnimationClip_GetSource(AnimationClip* o);

		static Texture2D* Material2D_GetTexture2D(Material2D* o, const achar* name);
		static Shader2D* Material2D_GetShader2D(Material2D* o);

		static AnimationClip* AnimationSyatem_CreateAnimationClip(AnimationSystem* o);
		static AnimationSource* AnimationSyatem_CreateAnimationSource(AnimationSystem* o);
		static KeyframeAnimation* AnimationSyatem_CreateKeyframeAnimation(AnimationSystem* o);
	};
}