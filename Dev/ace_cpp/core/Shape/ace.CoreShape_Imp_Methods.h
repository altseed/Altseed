bool GetIsCollidedWith(CoreShape* shape)
{
	if (GetType() == ShapeType::RectangleShape)
	{
		if (shape->GetShapeType() == ShapeType::CircleShape)
		{
			return GetIsCollidedWithCircleAndRect((CoreCircleShape*)shape, (CoreRectangleShape*)this);
		}
	}
	else if (GetType() == ShapeType::LineShape)
	{
		if (shape->GetShapeType() == ShapeType::CircleShape)
		{
			return GetIsCollidedWithCircleAndLine((CoreCircleShape*)shape, (CoreLineShape*)this);
		}
	}
	else if (GetType() == ShapeType::CircleShape)
	{
		if (shape->GetShapeType() == ShapeType::LineShape)
		{
			return GetIsCollidedWithCircleAndLine((CoreCircleShape*)this, (CoreLineShape*)shape);
		}
		else if (shape->GetShapeType() == ShapeType::RectangleShape)
		{
			return GetIsCollidedWithCircleAndRect((CoreCircleShape*)this, (CoreRectangleShape*)shape);
		}
	}
		
	return GetIsCollidedb2Shapes(shape);
}