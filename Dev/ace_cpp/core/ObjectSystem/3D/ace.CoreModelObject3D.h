
#pragma once

#include "ace.CoreObject3D.h"

namespace ace
{
	class CoreModelObject3D
		: public CoreObject3D
	{
	protected:
		CoreModelObject3D(){}
		virtual ~CoreModelObject3D(){}

	public:
		virtual void SetModel(Model* model) = 0;
		virtual void AddMesh(Mesh* mesh) = 0;
	};
}
