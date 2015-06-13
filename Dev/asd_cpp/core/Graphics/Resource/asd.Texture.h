
#pragma once

#include <asd.common.Base.h>
#include <Math/asd.Vector2DI.h>
#include "../../asd.ReferenceObject.h"

namespace asd
{
	class Texture
		: public IReference
	{
	protected:
		Texture() {}
		virtual ~Texture() {}

	public:

		/**
		@brief	テクスチャのクラスの種類を取得する。
		@return	種類
		*/
		virtual TextureClassType GetType() = 0;
	};
}
