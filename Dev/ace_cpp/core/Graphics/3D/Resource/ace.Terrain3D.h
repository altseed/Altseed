
#pragma once

#include "../../../ace.Core.Base.h"
#include "../../../ace.ReferenceObject.h"

namespace ace
{
	/**
	@brief	地形クラス
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

		virtual void AddSurface(const achar* name, float size, const achar* color, const achar* normal, const achar* specular) = 0;

		virtual int32_t GetSurfaceIndex(const achar* name) = 0;

		virtual void AssignSurfaceWithCircle(int32_t surfaceIndex, float x, float y, float radius, float value, float fallout) = 0;

	};
}