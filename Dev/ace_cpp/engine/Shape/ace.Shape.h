#pragma once

#include "../ace.CoreToEngine.h"

namespace ace
{
	class GeometryObject2D;
	class Layer2D;

	class Shape
	{
		friend class GeometryObject2D;
		friend class Layer2D;
	protected:
		virtual std::shared_ptr<CoreShape> GetCoreShape() const = 0;
	public:

		/**
		@brief	図形のタイプを取得する。
		@return	図形のタイプ
		*/
		virtual ShapeType GetShapeType() const = 0;

		bool GetIsCollidedWith(std::shared_ptr<Shape> shape);
	};
};