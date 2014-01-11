
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
		static AnimationSource* AnimationClip_GetSource(AnimationClip* o);

		static Texture2D* Material2D_GetTexture2D(Material2D* o, const achar* name);
		static Shader2D* Material2D_GetShader2D(Material2D* o);
	};
}