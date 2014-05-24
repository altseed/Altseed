
#pragma once

#include <ace.common.Base.h>
#include "ace.Texture.h"

namespace ace
{
	/**
	@brief	キューブマップ
	*/
	class CubemapTexture
		: public Texture
	{
	protected:
		CubemapTexture() {}
		virtual ~CubemapTexture() {}

	public:

		/**
		@brief	テクスチャのクラスの種類を取得する。
		@return	種類
		*/
		virtual eTextureClassType GetType() override { return TEXTURE_CLASS_CUBEMAPTEXTURE; }
	};
}