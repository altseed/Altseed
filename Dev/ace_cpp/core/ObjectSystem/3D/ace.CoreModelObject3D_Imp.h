
#pragma once

#include "ace.CoreModelObject3D.h"
#include "ace.CoreObject3D_Imp.h"

namespace ace
{
	class CoreModelObject3D_Imp
		: public CoreModelObject3D
		, public CoreObject3D_Imp
		, public ReferenceObject
	{
	private:
		RenderedModelObject3D*	m_object;

	public:
		CoreModelObject3D_Imp(Graphics* graphics);

	private:
		virtual ~CoreModelObject3D_Imp();
	public:

		void SetModel(Model* model) override;

		void AddMeshGroup() override;

		int32_t GetMeshGroupCount() override;

		void AddMesh(int32_t meshGroupIndex, Mesh* mesh) override;

		void SetDeformer(int32_t meshGroupIndex, Deformer* deformer) override;

		eRenderedObject3DType GetObjectType() { return eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_MESH; }


		// 共通部分
		virtual bool GetIsDrawn() const { return CoreObject3D_Imp::GetIsDrawn(); }
		virtual void SetIsDrawn(bool value) { return CoreObject3D_Imp::SetIsDrawn(value); }
		virtual Vector3DF GetPosition() const { return CoreObject3D_Imp::GetPosition(); }
		virtual void SetPosition(Vector3DF position) { CoreObject3D_Imp::SetPosition(position); }

		virtual Vector3DF GetRotation() const{ return CoreObject3D_Imp::GetRotation(); }
		virtual void SetRotation(const Vector3DF& rot){ CoreObject3D_Imp::SetRotation(rot); }

		virtual Vector3DF GetScale() const{ return CoreObject3D_Imp::GetScale(); }
		virtual void SetScale(const Vector3DF& scale){ CoreObject3D_Imp::SetScale(scale); }

		virtual void SetLayer(CoreLayer3D* layer) { CoreObject3D_Imp::SetLayer(layer); }

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif

	};
}
