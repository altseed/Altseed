
#pragma once

#include "ace.CoreObject3D.h"

namespace ace
{
	class CoreMassModelObject3D
		: public CoreObject3D
	{
	protected:
		CoreMassModelObject3D(){}
		virtual ~CoreMassModelObject3D(){}

	public:
		virtual void SetModel(MassModel* model) = 0;

		virtual void PlayAnimation(const achar* name) = 0;

		virtual void StopAnimation() = 0;

		virtual void CrossFadeAnimation(const achar* name, float time) = 0;

		virtual bool IsAnimationPlaying() = 0;
	};
}
