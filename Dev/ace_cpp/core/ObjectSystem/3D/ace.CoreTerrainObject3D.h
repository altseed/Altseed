
#pragma once

#include "ace.CoreObject3D.h"

namespace ace
{
	class CoreTerrainObject3D
		: public CoreObject3D
	{
	protected:
		CoreTerrainObject3D(){}
		virtual ~CoreTerrainObject3D(){}

	public:

		virtual void SetTerrain(Terrain3D* terrain) = 0;
	};
}
