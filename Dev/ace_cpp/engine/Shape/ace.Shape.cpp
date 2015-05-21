#include "ace.Shape.h"

namespace ace
{
	bool Shape::GetIsCollidedWith(Shape* shape)
	{
		GetCoreShape()->GetIsCollidedWith(shape->GetCoreShape().get());
	}
}
