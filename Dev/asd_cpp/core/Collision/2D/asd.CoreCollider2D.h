#pragma once

#include "asd.Collider2DType.h"
#include "../../asd.Core.Base.h"

namespace asd{

	class CoreCollider2D
		: public IReference
	{
	public:

		CoreCollider2D() {}
		virtual ~CoreCollider2D(){}

		virtual Collider2DShapeType GetType() const = 0;

		virtual const achar* GetTag() const = 0;
		virtual void SetTag(const achar*) = 0;

		virtual bool GetIsVisible() const = 0;
		virtual void SetIsVisible(bool isVisible) = 0;
	};
};
