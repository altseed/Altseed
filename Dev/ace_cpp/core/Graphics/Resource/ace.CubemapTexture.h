
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
			@brief	ミップマップ数を取得する。
			@return	ミップマップ数
		*/
		virtual int32_t GetMipmapCount() const = 0;

		/**
		@brief	テクスチャのクラスの種類を取得する。
		@return	種類
		*/
		virtual TextureClassType GetType() override { return TextureClassType::CubemapTexture; }
	};
}