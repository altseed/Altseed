
#pragma once

#include "ace.CorePostEffect.h"

namespace ace
{
	class CorePostEffect_Imp
		: public CorePostEffect
		, public ReferenceObject
	{
		friend class ObjectSystemFactory_Imp;

	private:
		PostEffectRenderer*	m_renderer;

		CorePostEffect_Imp(Graphics* graphics);
		virtual ~CorePostEffect_Imp();
	public:

		void DrawOnTexture2DWithMaterial(RenderTexture2D* target, Material2D* material);

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}