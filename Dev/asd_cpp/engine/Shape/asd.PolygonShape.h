#pragma once

#include "asd.Shape.h"

namespace asd
{
	/**
	@brief	図形としての多角形を扱うクラス。
	*/
	class PolygonShape :public Shape
	{
	private:
		std::shared_ptr<CorePolygonShape> m_coreObject;
	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;

	public:
		PolygonShape();
		virtual ~PolygonShape();

		/**
		@brief	ポリゴンを構成する頂点を追加する。
		@return	新規に追加する頂点の座標
		*/
		void AddVertex(Vector2DF vertex);

		/**
		@brief	ポリゴンを構成する頂点を全て削除する。
		*/
		void ClearVertexes();

		/**
		@brief	ポリゴンを構成する頂点の数量を取得する。
		@return	ポリゴンを構成する頂点の数量
		*/
		int GetVertexesNum() const;

		ShapeType GetShapeType() const override;
	};
}