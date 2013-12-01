
#pragma once

#include "ace.CoreMeshObject3D.h"
#include "ace.CoreObject3D_Imp.h"

namespace ace
{
	class CoreMeshObject3D_Imp
		: public CoreMeshObject3D
		, public CoreObject3D_Imp
		, public ReferenceObject
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


		// 共通部分
		virtual Vector3DF GetPosition() const { return CoreObject3D_Imp::GetPosition(); }
		virtual void SetPosition(Vector3DF position) { CoreObject3D_Imp::SetPosition(position); }
		virtual void SetLayer(CoreLayer3D* layer) { CoreObject3D_Imp::SetLayer(layer); }

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif

	};
}
