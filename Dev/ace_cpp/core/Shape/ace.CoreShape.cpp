#include "ace.CoreShape.h"
#include "ace.CoreTriangleShape.h"
#include <Box2D/Box2D.h>

#if _DEBUG
#pragma comment(lib,"Debug/Box2D.lib")
#else
#pragma comment(lib,"Release/Box2D.lib")
#endif

namespace ace
{
	CoreShape::~CoreShape()
	{
		for (auto triangle : triangles)
		{
			SafeRelease(triangle);
		}
	}

	bool CoreShape::GetIsCollidedWith(CoreShape* shape)
	{
		for (auto selfTriangle : GetDividedTriangles())
		{
			for (auto theirsTriangle : shape->GetDividedTriangles())
			{
				auto collidePolygon1 = new b2PolygonShape();
				auto collidePolygon2 = new b2PolygonShape();

				std::vector<b2Vec2> vec1, vec2;

				for (int index = 0; index < 3; ++index)
				{
					auto v1 = selfTriangle->GetPointByIndex(index);
					auto v2 = theirsTriangle->GetPointByIndex(index);
					vec1.push_back(b2Vec2(v1.X, v1.Y));
					vec2.push_back(b2Vec2(v2.X, v2.Y));
				}

				collidePolygon1->Set(vec1.data(), vec1.size());
				collidePolygon2->Set(vec2.data(), vec2.size());

				b2Transform identity = b2Transform();
				identity.SetIdentity();

				bool isOverlap = b2TestOverlap(collidePolygon1, 1, collidePolygon2, 1, identity, identity);

				delete collidePolygon1;
				delete collidePolygon2;

				if (isOverlap)
				{
					return true;
				}
			}
		}

		return false;
	}

#if !SWIG
	std::vector<CoreTriangleShape*>& CoreShape::GetDividedTriangles()
	{
		if (isNeededUpdating)
		{
			for (auto triangle : triangles)
			{
				SafeRelease(triangle);
			}
			triangles.clear();
			DivideToTriangles();
			isNeededUpdating = false;
		}
		return triangles;
	}

	culling2d::Circle& CoreShape::GetBoundingCircle()
	{
		if (isNeededCalcBoundingCircle)
		{
			CalculateBoundingCircle();
			isNeededCalcBoundingCircle = false;
		}
		return boundingCircle;
	}
#endif
};