
#pragma once

#include "asd.CoreMassModelObject3D.h"
#include "asd.CoreObject3D_Imp.h"

#include <Math/asd.Vector3DF.h>

namespace asd
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
		void SetMaterialPropertyBlock(MaterialPropertyBlock* block) override;

		void SetModel(MassModel* model) override;

		void PlayAnimation(const achar* name) override;

		void StopAnimation() override;

		void CrossFadeAnimation(const achar* name, float time) override;

		bool GetIsAnimationPlaying() override;

		RenderedObject3DType GetObjectType() override { return RenderedObject3DType::MassObject; }

		// 共通部分
		virtual bool GetIsDrawn() const { return CoreObject3D_Imp::GetIsDrawn(); }
		virtual void SetIsDrawn(bool value) { return CoreObject3D_Imp::SetIsDrawn(value); }
		virtual Vector3DF GetPosition() const { return CoreObject3D_Imp::GetPosition(); }
		virtual void SetPosition(Vector3DF position) { CoreObject3D_Imp::SetPosition(position); }

		virtual Vector3DF GetRotation() const{ return CoreObject3D_Imp::GetRotation(); }
		virtual void SetRotation(const Vector3DF& rot){ CoreObject3D_Imp::SetRotation(rot); }

		virtual Vector3DF GetScale() const{ return CoreObject3D_Imp::GetScale(); }
		virtual void SetScale(const Vector3DF& scale){ CoreObject3D_Imp::SetScale(scale); }

		virtual Matrix44 GetLocalMatrix() { return CoreObject3D_Imp::GetLocalMatrix(); }
		virtual void SetLoalMatrixDirectly(Matrix44 localMat) { CoreObject3D_Imp::SetLoalMatrixDirectly(localMat); }

		virtual void SetLayer(CoreLayer3D* layer) { CoreObject3D_Imp::SetLayer(layer); }

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}
