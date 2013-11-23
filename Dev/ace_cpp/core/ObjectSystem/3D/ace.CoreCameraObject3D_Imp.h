
#pragma once

#include "ace.CoreCameraObject3D.h"
#include "ace.CoreObject3D_Imp.h"

namespace ace
{
	class CoreCameraObject3D_Imp
		: public CoreCameraObject3D
		, public CoreObject3D_Imp
	{
	private:
		RenderedCameraObject3D*	m_object;

	public:
		CoreCameraObject3D_Imp(Graphics* graphics);

	private:
		virtual ~CoreCameraObject3D_Imp();
	public:

		eRenderedObject3DType GetObjectType() { return eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_CAMERA; }
	};
}
