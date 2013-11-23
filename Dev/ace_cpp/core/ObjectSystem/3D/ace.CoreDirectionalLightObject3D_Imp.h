
#pragma once

#include "ace.CoreDirectionalLightObject3D.h"
#include "ace.CoreObject3D_Imp.h"

namespace ace
{
	class CoreDirectionalLightObject3D_Imp
		: public CoreDirectionalLightObject3D
		, public CoreObject3D_Imp
	{
	private:
		RenderedDirectionalLightObject3D*	m_object;

	public:
		CoreDirectionalLightObject3D_Imp(Graphics* graphics);

	private:
		virtual ~CoreDirectionalLightObject3D_Imp();
	public:

		eRenderedObject3DType GetObjectType() { return eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_DIRECTIONALLIGHT; }
	};
}
