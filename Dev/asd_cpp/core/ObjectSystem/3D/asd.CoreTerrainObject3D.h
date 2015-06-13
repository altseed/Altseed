
#pragma once

#include "asd.CoreObject3D.h"

namespace asd
{
	class CoreTerrainObject3D
		: public CoreObject3D
	{
	protected:
		CoreTerrainObject3D(){}
		virtual ~CoreTerrainObject3D(){}

	public:
		virtual void SetMaterialPropertyBlock(MaterialPropertyBlock* block) = 0;

		virtual void SetTerrain(Terrain3D* terrain) = 0;
	};
}
