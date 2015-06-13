
#pragma once

#include "../../asd.Core.Base_Imp.h"

namespace asd
{
	class CoreObject3D
		: public IReference
	{
	protected:
		CoreObject3D(){}
		virtual ~CoreObject3D(){}

	public:

		virtual bool GetIsDrawn() const = 0;
		virtual void SetIsDrawn(bool value) = 0;

		virtual Vector3DF GetPosition() const = 0;
		virtual void SetPosition(Vector3DF position) = 0;
		virtual Vector3DF GetRotation() const = 0;
		virtual void SetRotation(const Vector3DF& rot) = 0;
		virtual Vector3DF GetScale() const = 0;
		virtual void SetScale(const Vector3DF& scale) = 0;

#if !SWIG
		virtual void SetLayer(CoreLayer3D* layer) = 0;
#endif

		virtual RenderedObject3DType GetObjectType() = 0;

	};
}
