
#pragma once

#include "ace.CoreDirectionalLightObject3D.h"
#include "ace.CoreObject3D_Imp.h"

namespace ace
{
	class CoreDirectionalLightObject3D_Imp
		: public CoreDirectionalLightObject3D
		, public CoreObject3D_Imp
		, public ReferenceObject
	{
	private:
		RenderedDirectionalLightObject3D*	m_object;

	public:
		CoreDirectionalLightObject3D_Imp(Graphics* graphics);

	private:
		virtual ~CoreDirectionalLightObject3D_Imp();
	public:

		Color GetColor();
		void SetColor(const Color& color);

		eRenderedObject3DType GetObjectType() { return eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_DIRECTIONALLIGHT; }



		// 共通部分
		virtual bool GetIsDrawn() const { return CoreObject3D_Imp::GetIsDrawn(); }
		virtual void SetIsDrawn(bool value) { return CoreObject3D_Imp::SetIsDrawn(value); }
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
