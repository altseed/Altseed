#include "asd.CoreShapeConverter.h"
#include "asd.CoreArcShape_Imp.h"
#include "asd.CoreCircleShape_Imp.h"
#include "asd.CoreLineShape_Imp.h"
#include "asd.CorePolygonShape_Imp.h"
#include "asd.CoreRectangleShape_Imp.h"
#include "asd.CoreTriangleShape_Imp.h"

namespace asd
{

	CoreShape_Imp* CoreShape2DToImp(CoreShape* obj)
	{
		if (obj == nullptr) return nullptr;

		switch (obj->GetShapeType())
		{
		case ShapeType::ArcShape:
			return (CoreArcShape_Imp*)obj;
		case ShapeType::CircleShape:
			return (CoreCircleShape_Imp*)obj;
		case ShapeType::LineShape:
			return (CoreLineShape_Imp*)obj;
		case ShapeType::RectangleShape:
			return (CoreRectangleShape_Imp*)obj;
		case ShapeType::PolygonShape:
			return (CorePolygonShape_Imp*)obj;
		case ShapeType::TriangleShape:
			return (CoreTriangleShape_Imp*)obj;
		default:
			break;
		}

		return nullptr;
	}
}