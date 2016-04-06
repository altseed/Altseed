
#pragma once

#include "asd.CoreTerrainObject3D.h"
#include "asd.CoreObject3D_Imp.h"

namespace asd
{
	class CoreTerrainObject3D_Imp
		: public CoreTerrainObject3D
		, public CoreObject3D_Imp
		, public ReferenceObject
	{
	private:
		RenderedTerrainObject3D*	m_object;

	public:
		CoreTerrainObject3D_Imp(Graphics* graphics);

	private:
		virtual ~CoreTerrainObject3D_Imp();
	public:
		void SetMaterialPropertyBlock(MaterialPropertyBlock* block) override;

		void SetTerrain(Terrain3D* terrain) override;

		RenderedObject3DType GetObjectType() { return RenderedObject3DType::Terrain; }

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
