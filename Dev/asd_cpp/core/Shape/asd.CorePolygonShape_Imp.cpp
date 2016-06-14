#include "asd.CorePolygonShape_Imp.h"
#include "asd.CoreTriangleShape_Imp.h"
#include "../3rdParty/poly2tri/poly2tri.h"
#include <Box2D/Box2D.h>
#include "asd.CoreShapeConverter.h"

namespace asd
{
	CorePolygonShape_Imp::CorePolygonShape_Imp()
	{

	}

	CorePolygonShape_Imp::~CorePolygonShape_Imp()
	{
		std::for_each(holeShapes.begin(), holeShapes.end(), [](CoreShape* holeShape){SafeRelease(holeShape); });
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

	bool CorePolygonShape_Imp::AddHole(CoreShape *holeShape)
	{
		if (std::find(holeShapes.begin(), holeShapes.end(), holeShape) != holeShapes.end())
		{
			return false;
		}

		SafeAddRef(holeShape);
		holeShapes.push_back(holeShape);
		holeShapes_Imp.push_back(CoreShape2DToImp(holeShape));
		return true;
	}

	bool CorePolygonShape_Imp::RemoveHole(CoreShape *holeShape)
	{
		if (std::find(holeShapes.begin(), holeShapes.end(), holeShape) == holeShapes.end())
		{
			return false;
		}

		SafeRelease(holeShape);
		holeShapes.erase(std::remove(holeShapes.begin(), holeShapes.end(), holeShape), holeShapes.end());
		holeShapes_Imp.erase(std::remove(holeShapes_Imp.begin(), holeShapes_Imp.end(), CoreShape2DToImp(holeShape)), holeShapes_Imp.end());
		return true;
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

		std::vector < std::vector<p2t::Point* >> holePointsList;
		p2t::CDT* cdt = new p2t::CDT(points);
		
		for (auto holeShape : holeShapes_Imp)
		{

			for (auto tri : holeShape->GetDividedTriangles())
			{
				std::vector<p2t::Point*> holePoints(3);
				for (int p = 0; p < 3; ++p)
				{
					auto holePoint = tri->GetPointByIndex(p);
					holePoints[p] = new p2t::Point(holePoint.X, holePoint.Y);
				}
				cdt->AddHole(holePoints);
				holePointsList.push_back(holePoints);
			}
		}

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

		for (auto holePoints : holePointsList)
		{
			for (auto holePoint : holePoints)
			{
				delete holePoint;
			}
		}

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