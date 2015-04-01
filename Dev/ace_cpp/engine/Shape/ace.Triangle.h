#pragma once

#include "ace.Shape.h"

namespace ace
{
	class Triangle :public Shape
	{
	private:
		std::shared_ptr<CoreTriangle> m_coreObject;

	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;
	public:
		Triangle();
		virtual ~Triangle();

		Vector2DF GetPointByIndex(int index) const;
		void SetPointByIndex(Vector2DF point, int index);

		Vector2DF GetUVByIndex(int index) const;
		void SetUVByIndex(Vector2DF uv, int index);

		ShapeType GetShapeType() const override;
	};
}