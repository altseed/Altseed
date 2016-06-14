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
		std::vector<std::shared_ptr<CoreShape>> holeShapes;

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
		@brief	多角形に空ける穴となるオブジェクトを追加する。
		@param	holeShape	穴となるオブジェクトのインスタンス
		@return	正しく追加されたかどうか
		*/
		bool AddHole(std::shared_ptr<CoreShape> holeShape);

		/**
		@brief	多角形に空ける穴となるオブジェクトを削除する。
		@param	holeShape	穴となるオブジェクトのインスタンス
		@return	正しく削除されたかどうか
		*/
		bool RemoveHole(std::shared_ptr<CoreShape> holeShape);

		/**
		@brief	追加された穴となるオブジェクトの一覧を取得する。
		@return	追加された穴となるオブジェクトの一覧
		*/
		std::vector<std::shared_ptr<CoreShape>>& GetHoleShapes();

		/**
		@brief	ポリゴンを構成する頂点の数量を取得する。
		@return	ポリゴンを構成する頂点の数量
		*/
		int GetVertexesNum() const;

		ShapeType GetShapeType() const override;
	};
}