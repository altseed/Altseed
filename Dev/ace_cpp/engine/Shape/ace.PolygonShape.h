#pragma once

#include "ace.Shape.h"

namespace ace
{
	class PolygonShape :public Shape
	{
	private:
		std::shared_ptr<CorePolygonShape> m_coreObject;
	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;

	public:
		PolygonShape();
		virtual ~PolygonShape();

		void AddVertex(Vector2DF vertex);
		void ClearVertexes();
		int GetVertexesNum() const;

		ShapeType GetShapeType() const override;
	};
}