#include "asd.CorePolygonShape_Imp.h"
#include "asd.CoreTriangleShape_Imp.h"
#include "../3rdParty/poly2tri/poly2tri.h"
#include <Box2D/Box2D.h>

namespace asd
{
	CorePolygonShape_Imp::CorePolygonShape_Imp()
	{

	}

	CorePolygonShape_Imp::~CorePolygonShape_Imp()
	{

	}

	bool CorePolygonShape_Imp::IsValid()
	{
		return isValid;
	}

	void CorePolygonShape_Imp::AddVertex(Vector2DF vertex)
	{
		isNeededCalcBoundingCircle = true;
		isNeededUpdating = true;
		isNeededCalcCollisions = true;
		NotifyUpdateToObject();
		vertexes.push_back(vertex);
	}

	void CorePolygonShape_Imp::ClearVertexes()
	{
		isNeededCalcBoundingCircle = true;
		isNeededUpdating = true;
		isNeededCalcCollisions = true;
		NotifyUpdateToObject();
		vertexes.clear();
	}

	int CorePolygonShape_Imp::GetVertexesNum() const
	{
		return (int)vertexes.size();
	}

	ShapeType CorePolygonShape_Imp::GetShapeType() const
	{
		return ShapeType::PolygonShape;
	}

	ShapeType CorePolygonShape_Imp::GetType() const
	{
		return ShapeType::PolygonShape;
	}

#if !SWIG
	void CorePolygonShape_Imp::DivideToTriangles()
	{
		isValid = true;

		float maxLeft = FLT_MAX;
		float maxRight = -FLT_MAX;
		float maxHigh = FLT_MAX;
		float maxLow = -FLT_MAX;

		for (auto vertex : vertexes)
		{
			maxLeft = Min(maxLeft, vertex.X);
			maxRight = Max(maxRight, vertex.X);
			maxHigh = Min(maxHigh, vertex.Y);
			maxLow = Max(maxLow, vertex.Y);
		}

		std::vector<p2t::Point*> points;

		for (auto vertex : vertexes)
		{
			p2t::Point* point = new p2t::Point(vertex.X, vertex.Y);
			points.push_back(point);
		}

		// 重畳している場合は失敗扱いにする。
		std::set<Vector2DF> pset;
		for (auto p : points)
		{
			auto v = Vector2DF((*p).x, (*p).y);
			if (pset.find(v) != pset.end())
			{
				for (auto point : points)
				{
					delete point;
				}
				isValid = false;
				return;
			}
			pset.insert(v);
		}

		p2t::CDT* cdt = new p2t::CDT(points);
		cdt->Triangulate();

		auto outTriangles = cdt->GetTriangles();

		for (auto tri : outTriangles)
		{
			CoreTriangleShape* triangle = new CoreTriangleShape_Imp();
			for (int i = 0; i < 3; ++i)
			{
				auto p = tri->GetPoint(i);
				triangle->SetPointByIndex(Vector2DF(p->x, p->y), i);

				float uvX = (p->x - maxLeft) / (maxRight - maxLeft);
				float uvY = (p->y - maxHigh) / (maxLow - maxHigh);
				triangle->SetUVByIndex(Vector2DF(uvX, uvY), i);
			}
			triangles.push_back(triangle);
		}


		delete cdt;

		for (auto point : points)
		{
			delete point;
		}
	}

	void CorePolygonShape_Imp::CalculateBoundingCircle()
	{
		float maxLeft = FLT_MAX;
		float maxRight = -FLT_MAX;
		float maxHigh = FLT_MAX;
		float maxLow = -FLT_MAX;

		for (auto vertex : vertexes)
		{
			maxLeft = Min(maxLeft, vertex.X);
			maxRight = Max(maxRight, vertex.X);
			maxHigh = Min(maxHigh, vertex.Y);
			maxLow = Max(maxLow, vertex.Y);
		}

		auto center = (Vector2DF(maxLeft, maxHigh) + Vector2DF(maxRight, maxLow)) / 2.0f;
		auto radius = (Vector2DF(maxLeft, maxHigh) - center).GetLength();

		boundingCircle = culling2d::Circle(culling2d::Vector2DF(center.X, center.Y), radius);
	}

	void CorePolygonShape_Imp::CalcCollisions()
	{
		std::vector<b2Vec2> polyPoints;
		for (auto vertex : vertexes)
		{
			polyPoints.push_back(b2Vec2(vertex.X, vertex.Y));
		}

		auto polygon = new b2PolygonShape();

		polygon->Set(polyPoints.data(), polyPoints.size());

		collisionShapes.push_back(polygon);
	}
#endif
}