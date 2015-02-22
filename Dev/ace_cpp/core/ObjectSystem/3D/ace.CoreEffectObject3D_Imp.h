
#pragma once

#include "ace.CoreEffectObject3D.h"
#include "ace.CoreObject3D_Imp.h"

namespace ace
{
	class CoreEffectObject3D_Imp
		: public CoreEffectObject3D
		, public CoreObject3D_Imp
		, public ReferenceObject
	{
	private:
		RenderedEffectObject3D*	m_object;

	public:
		CoreEffectObject3D_Imp(Graphics* graphics);

	private:
		virtual ~CoreEffectObject3D_Imp();
	public:

		void SetEffect(Effect* effect) override;

		void Play() override;

		void Stop() override;

		void StopRoot() override;

		bool IsPlaying() override;

		bool GetSyncEffects() override;

		void SetSyncEffects(bool value) override;

		RenderedObject3DType GetObjectType() { return RenderedObject3DType::Effect; }

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
