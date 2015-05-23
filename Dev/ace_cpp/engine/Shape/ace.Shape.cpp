#include "ace.Shape.h"

namespace ace
{
	bool Shape::GetIsCollidedWith(Shape* shape)
	{
		return GetCoreShape()->GetIsCollidedWith(shape->GetCoreShape().get());
	}
}
