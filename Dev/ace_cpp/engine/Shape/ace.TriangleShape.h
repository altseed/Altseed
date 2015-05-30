#pragma once

#include "ace.Shape.h"

namespace ace
{
	/**
	@brief	図形としての三角形を扱うクラス。
	*/
	class TriangleShape :public Shape
	{
	private:
		std::shared_ptr<CoreTriangleShape> m_coreObject;

	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;
	public:
		TriangleShape();
		virtual ~TriangleShape();

		/**
		@brief	指定したインデックスの頂点の座標を取得する。
		@param	index	座標を取得する頂点のインデックス（0 <= index < 3）
		@return	頂点の座標
		*/
		Vector2DF GetPointByIndex(int index) const;

		/**
		@brief	指定したインデックスの頂点の座標を設定する。
		@param	point	インデックスで指定した頂点に設定する座標
		@param	index	座標を設定する頂点のインデックス（0 <= index < 3）
		*/
		void SetPointByIndex(Vector2DF point, int index);

		/**
		@brief	指定したインデックスの頂点のUVを取得する。
		@param	index	UVを取得する頂点のインデックス（0 <= index < 3）
		@return	頂点のUV
		*/
		Vector2DF GetUVByIndex(int index) const;

		/**
		@brief	指定したインデックスの頂点のUVを設定する。
		@param	uv	インデックスで指定した頂点に設定するUV
		@param	index	UVを設定する頂点のインデックス（0 <= index < 3）
		*/
		void SetUVByIndex(Vector2DF uv, int index);

		ShapeType GetShapeType() const override;
	};
}