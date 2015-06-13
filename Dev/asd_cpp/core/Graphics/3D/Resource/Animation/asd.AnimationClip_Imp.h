
#pragma once

#include "../../../../asd.Core.Base_Imp.h"
#include "asd.AnimationClip.h"

namespace asd
{
	class AnimationClip_Imp
		: public AnimationClip
		, public ReferenceObject
	{
	private:
		AnimationSource*	m_source;
		bool				isLoopingMode = false;

	public:

		AnimationClip_Imp();
		virtual ~AnimationClip_Imp();

		AnimationSource* GetSource_();
		void SetSource(AnimationSource* source);

		bool GetIsLoopingMode() const override;

		void SetIsLoopingMode(bool isLoopingMode) override;

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}