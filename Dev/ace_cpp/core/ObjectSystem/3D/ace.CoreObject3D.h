
#pragma once

#include "../../ace.Core.Base_Imp.h"

namespace ace
{
	class CoreObject3D
		: public IReference
	{
	public:

		virtual Vector3DF GetPosition() const = 0;
		virtual void SetPosition(Vector3DF position) = 0;

#if !SWIG
		virtual void SetLayer(CoreLayer3D* layer) = 0;
#endif

		virtual eRenderedObject3DType GetObjectType() = 0;

	};
}
