#pragma once
#include "asd.Collider2D.h"

namespace asd {

	/**
	矩形のコライダクラス
	*/
	class RectangleCollider : public Collider2D{
	private:
		std::shared_ptr<CoreRectangleCollider> coreObject;
		std::shared_ptr<CoreCollider2D> GetCoreCollider() const override;
	public:
		RectangleCollider();
		virtual ~RectangleCollider();

		/**
		@brief	この矩形の範囲を取得する
		@return	矩形の範囲
		*/
		RectF GetArea() const;

		/**
		@brief	この矩形の範囲を設定する
		@param	area	矩形の範囲
		*/
		void SetArea(RectF area);

		Collider2DShapeType GetShapeType() const override;
	};
}