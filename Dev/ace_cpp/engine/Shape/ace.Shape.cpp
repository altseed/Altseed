#include "ace.Shape.h"

namespace ace
{
	bool Shape::GetIsCollidedWith(std::shared_ptr<Shape> shape)
	{
		return GetCoreShape()->GetIsCollidedWith(shape->GetCoreShape().get());
	}
}
