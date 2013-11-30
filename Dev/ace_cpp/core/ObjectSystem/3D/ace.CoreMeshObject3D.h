
#pragma once

#include "ace.CoreObject3D.h"

namespace ace
{
	class CoreMeshObject3D
		: public CoreObject3D
	{
	protected:
		CoreMeshObject3D(){}
		virtual ~CoreMeshObject3D(){}

	public:

		virtual void SetMesh(Mesh* mesh) = 0;
	};
}
