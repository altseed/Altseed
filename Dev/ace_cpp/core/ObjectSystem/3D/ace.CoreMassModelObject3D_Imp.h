
#pragma once

#include "ace.CoreMassModelObject3D.h"
#include "ace.CoreObject3D_Imp.h"

#include <Math/ace.Vector3DF.h>

namespace ace
{
	class CoreMassModelObject3D_Imp
		: public CoreMassModelObject3D
		, public CoreObject3D_Imp
		, public ReferenceObject
	{
	protected:
		RenderedMassModelObject3D*	m_object = nullptr;

	public:
		CoreMassModelObject3D_Imp(Graphics* graphics);

	private:
		virtual ~CoreMassModelObject3D_Imp();

	public:
		void SetModel(MassModel* model) override;

		eRenderedObject3DType GetObjectType() override { return eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_MASSOBJECT; }

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
