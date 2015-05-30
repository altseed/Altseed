#pragma once

#include "../ace.CoreToEngine.h"

namespace ace
{
	class GeometryObject2D;
	class Layer2D;

	/**
	@brief	当たり判定や図形描画に用いられる図形を扱うクラス。
	*/
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

		/**
		@brief 指定した図形と衝突しているか否かを取得する。
		@param shape 衝突を調べる図形
		@return 衝突しているか否か
		*/
		bool GetIsCollidedWith(std::shared_ptr<Shape> shape);
	};
};