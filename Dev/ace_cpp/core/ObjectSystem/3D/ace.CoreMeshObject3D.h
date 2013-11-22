
#pragma once

#include "ace.CoreObject3D.h"

namespace ace
{
	class CoreMeshObject3D
		: public CoreObject3D
	{
	public:
		CoreMeshObject3D(){}
		virtual ~CoreMeshObject3D(){}

		virtual void SetMesh(Mesh* mesh) = 0;
	};
}
