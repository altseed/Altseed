
#pragma once

#include "ace.CoreMeshObject3D.h"
#include "../../Graphics/Common/3D/ace.RenderedMeshObject3D.h"

namespace ace
{
	class CoreMeshObject3D_Imp
		: public CoreMeshObject3D
	{
	private:
		RenderedMeshObject3D*	m_object;

	public:
		CoreMeshObject3D_Imp(Graphics* graphics);

	private:
		virtual ~CoreMeshObject3D_Imp();
	public:

		void SetLayer(CoreLayer3D* layer);

		RenderedMeshObject3D* GetInternalObject(){ return m_object; }

		eRenderedObject3DType GetObjectType() { return eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_MESH; }

	};
}
