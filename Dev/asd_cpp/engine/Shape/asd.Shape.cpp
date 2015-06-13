#include "asd.Shape.h"

namespace asd
{
	bool Shape::GetIsCollidedWith(std::shared_ptr<Shape> shape)
	{
		return GetCoreShape()->GetIsCollidedWith(shape->GetCoreShape().get());
	}
}
