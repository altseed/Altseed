
#pragma once

#include "ace.CoreObject3D.h"

namespace ace
{
	class CoreMassModelObject3D
		: public CoreObject3D
	{
	protected:
		CoreMassModelObject3D(){}
		virtual ~CoreMassModelObject3D(){}

	public:
		virtual void SetModel(MassModel* model) = 0;
	};
}
