
#pragma once

#include "../../../ace.Core.Base.h"
#include "../../../ace.ReferenceObject.h"

namespace ace
{
	/**
	@brief	地形のクラス
	*/
	class Terrain3D
		: public IReference
	{
		friend class Accessor;
	protected:
		Terrain3D(){}
		virtual ~Terrain3D(){}

	public:
		/**
			@brief	新規に地形を作成する。
			@param	gridSize		グリッド1つ当たりの大きさ
			@param	gridWidthCount	横方向のグリッド数
			@param	gridHeightCount	縦方向のグリッド数
		*/
		virtual void New(float gridSize, int32_t gridWidthCount, int32_t gridHeightCount) = 0;

		/**
			@brief	素材を追加する。
			@param	name	素材名
			@param	size	素材のテクスチャ1枚あたりの大きさ
			@param	color	色テクスチャのパス
			@param	normal	法線テクスチャのパス
			@param	metalness	スペキュラテクスチャのパス
		*/
		virtual void AddSurface(const achar* name, float size, const achar* color, const achar* normal, const achar* metalness) = 0;

		/**
			@brief	素材インデックスを取得する。
			@param	name	素材名
			@return	素材インデックス
		*/
		virtual int32_t GetSurfaceIndex(const achar* name) = 0;

		/**
			@brief	素材を円形に割り当てる。
			@param	surfaceIndex	素材インデックス
			@param	x	円形の中心座標
			@param	y	円形の中心座標
			@param	radius	円形の半径
			@param	value	割り当てる値(-255～255)
			@param	fallout	周囲のぼかし(0～1)
		*/
		virtual void AssignSurfaceWithCircle(int32_t surfaceIndex, float x, float y, float radius, float value, float fallout) = 0;

		/**
		@brief	材質を設定する。
		@param	material		材質
		*/
		virtual void SetMaterial(Material3D* material) = 0;

		/**
		@brief	円形に地形を上下せる。
		@param	x	円形の中心座標
		@param	y	円形の中心座標
		@param	radius	円形の半径
		@param	value	値
		@param	fallout	周囲のぼかし(0～1)
		*/
		virtual void RaiseWithCircle(float x, float y, float radius, float value, float fallout) = 0;

#if !SWIG
		/**
		@brief	材質を設定する。
		@param	material		材質
		*/
		void SetMaterial(std::shared_ptr<Material3D> material)
		{
			SetMaterial(material.get());
		}
#endif
	};
}