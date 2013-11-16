
#pragma once

#include "ace.CoreLayer.h"

namespace ace
{
	class CoreLayer_Imp 
		: public CoreLayer
	{
	protected:

		bool m_isDrawn;

		CoreLayer_Imp();
		virtual ~CoreLayer_Imp();
	public:

		virtual bool GetIsDrawn() const;
		virtual void SetIsDrawn(bool value);
	};
}