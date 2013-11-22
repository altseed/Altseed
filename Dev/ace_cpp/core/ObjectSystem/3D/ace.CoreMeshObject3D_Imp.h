
#pragma once

#include "ace.CoreMeshObject3D.h"
#include "ace.CoreObject3D_Imp.h"

namespace ace
{
	class CoreMeshObject3D_Imp
		: public CoreMeshObject3D
		, public CoreObject3D_Imp
	{
	private:
		RenderedMeshObject3D*	m_object;

	public:
		CoreMeshObject3D_Imp(Graphics* graphics);

	private:
		virtual ~CoreMeshObject3D_Imp();
	public:

		void SetMesh(Mesh* mesh);

		eRenderedObject3DType GetObjectType() { return eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_MESH; }
	};
}
