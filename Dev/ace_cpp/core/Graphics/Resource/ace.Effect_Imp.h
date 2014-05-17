
#pragma once

#include "ace.Effect.h"

namespace ace
{
	class Effect_Imp
		: public Effect
		, public ReferenceObject
	{
	private:
		Effekseer::Effect*	m_effect;

		Effect_Imp(Effekseer::Effect* effect);
		virtual ~Effect_Imp();
	public:

		static Effect_Imp* CreateEffect(Graphics* graphics, Effekseer::Effect* effect);
		
		Effekseer::Effect* GetEffect() { return m_effect; }

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}