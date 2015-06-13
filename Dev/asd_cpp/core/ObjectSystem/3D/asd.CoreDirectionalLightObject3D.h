
#pragma once

#include "asd.CoreObject3D.h"

#include <Graphics/asd.Color.h>

namespace asd
{
	class CoreDirectionalLightObject3D
		: public CoreObject3D
	{
	protected:
		CoreDirectionalLightObject3D(){}
		virtual ~CoreDirectionalLightObject3D(){}

	public:
		virtual Color GetColor() = 0;
		virtual void SetColor(const Color& color) = 0;

		virtual float GetIntensity() = 0;
		virtual void SetIntensity(float intensity) = 0;
	};
}
