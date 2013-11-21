
#pragma once

#include "../../ace.Core.Base_Imp.h"

namespace ace
{
	class CoreObject3D
		: public IReference
	{
	public:
		virtual void SetLayer(CoreLayer3D* layer) = 0;

		virtual eRenderedObject3DType GetObjectType() = 0;

	};
}
