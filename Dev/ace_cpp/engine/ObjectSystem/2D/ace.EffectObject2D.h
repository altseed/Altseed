#pragma once

#include "ace.Object2D.h"

namespace ace
{
	class EffectObject2D : public Object2D
	{
	private:
		std::shared_ptr<CoreEffectObject2D> m_coreObject;

		CoreObject2D* GetCoreObject() const;

	public:
		typedef std::shared_ptr<EffectObject2D> Ptr;

		EffectObject2D();
		virtual ~EffectObject2D();



	};
}