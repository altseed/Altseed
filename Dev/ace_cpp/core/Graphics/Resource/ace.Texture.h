
#pragma once

#include <ace.common.Base.h>
#include <Math/ace.Vector2DI.h>
#include "../../ace.ReferenceObject.h"

namespace ace
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
