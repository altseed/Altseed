#pragma once
#include "asd.Collider2D.h"

namespace asd {

	/**
	ポリゴンのコライダクラス
	*/
	class PolygonCollider : public Collider2D
	{
	private:
		std::shared_ptr<CorePolygonCollider> coreObject;
		std::shared_ptr<CoreCollider2D> GetCoreCollider() const override;
	public:
		PolygonCollider();
		~PolygonCollider();

		/**
		@brief	このポリゴンが正しい形式であることの成否を取得する
		@return	このポリゴンが正しい形式であることの成否
		*/
		bool IsValid();

		/**
		@brief	このポリゴンに新たな頂点を追加する
		@param	vertex	追加する新たな頂点の座標
		*/
		void AddVertex(Vector2DF vertex);


		/**
		@brief	このポリゴンの頂点全てをクリアする
		*/
		void ClearVertexes();

		/**
		@brief	このポリゴンの頂点数を取得する
		@return	ポリゴンの頂点数
		*/
		int GetVertexesNum() const;

		/**
		@brief	このポリゴンのindex番目の頂点位置を設定する
		@param	index	ポリゴンの頂点のインデックス
		@param	vertex	設定する頂点座標
		*/
		void SetVertex(int index, Vector2DF vertex);

		/**
		@brief	このポリゴンのindex番目の頂点位置を取得する
		@param	index	ポリゴンの頂点のインデックス
		@param	vertex	取得する頂点座標
		*/
		Vector2DF GetVertex(int index) const;

		Collider2DShapeType GetShapeType() const override;
	};
}