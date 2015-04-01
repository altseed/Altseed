#include "ace.CoreRectangle_Imp.h"
#include "ace.CoreTriangle_Imp.h"

namespace ace
{

	CoreRectangle_Imp::CoreRectangle_Imp()
		:drawingArea(RectF())
		, uv(RectF())
		, angle(0)
	{

	}

	RectF CoreRectangle_Imp::GetDrawingArea() const
	{
		return drawingArea;
	}

	void CoreRectangle_Imp::SetDrawingArea(RectF drawingArea)
	{
		isNeededUpdating = true;
		isNeededCalcBoundingCircle = true;
		this->drawingArea = drawingArea;
	}

	RectF CoreRectangle_Imp::GetUV() const
	{
		return uv;
	}

	void CoreRectangle_Imp::SetUV(RectF uv)
	{
		isNeededUpdating = true;
		this->uv = uv;
	}

	float CoreRectangle_Imp::GetAngle() const
	{
		return angle;
	}

	void CoreRectangle_Imp::SetAngle(float angle)
	{
		isNeededUpdating = true;
		isNeededCalcBoundingCircle = true;
		this->angle = angle;
	}

	Vector2DF CoreRectangle_Imp::GetCenterPosition() const
	{
		return centerPosition;
	}

	void CoreRectangle_Imp::SetCenterPosition(Vector2DF centerPosition)
	{
		isNeededUpdating = true;
		isNeededCalcBoundingCircle = true;
		this->centerPosition = centerPosition;
	}

	ShapeType CoreRectangle_Imp::GetShapeType() const
	{
		return ShapeType::Rectangle;
	}

#if !SWIG
	void CoreRectangle_Imp::DivideToTriangles()
	{
		auto vertexes = drawingArea.GetVertexes();

		auto globalCenter = vertexes[0] + centerPosition;

		for (auto& vert : vertexes)
		{
			vert -= globalCenter;
			auto deg = vert.GetDegree();
			deg += angle;
			vert.SetDegree(deg);
			vert += globalCenter;
		}

		auto uvs = uv.GetVertexes();

		auto triangle1 = new CoreTriangle_Imp();
		auto triangle2 = new CoreTriangle_Imp();

		for (int j = 0; j < 3; ++j)
		{
			triangle1->SetPointByIndex(vertexes[j], j);
			triangle1->SetUVByIndex(uvs[j], j);

			triangle2->SetPointByIndex(vertexes[j + 1], j + 1);
			triangle2->SetUVByIndex(uvs[j + 1], j + 1);
		}

		triangles.push_back(triangle1);
		triangles.push_back(triangle2);
	}

	void CoreRectangle_Imp::CalculateBoundingCircle()
	{
		std::array<Vector2DF, 4> vertexes = drawingArea.GetVertexes();

		auto globalCenter = vertexes[0] + centerPosition;

		for (auto& vertex : vertexes)
		{
			vertex -= globalCenter;
			auto deg = vertex.GetDegree();
			deg += angle;
			vertex.SetDegree(deg);
			vertex += globalCenter;
		}

		Vector2DF center = (vertexes[0] + vertexes[1] + vertexes[2] + vertexes[3]) / 4.0f;
		float radius = (vertexes[0] - center).GetLength();

		boundingCircle = culling2d::Circle(culling2d::Vector2DF(center.X, center.Y), radius);
	}
#endif

}
