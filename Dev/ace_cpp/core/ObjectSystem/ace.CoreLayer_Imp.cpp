
#include "ace.CoreLayer_Imp.h"

namespace ace
{

	CoreLayer_Imp::CoreLayer_Imp()
		: m_isDrawn(true)
	{
	}

	CoreLayer_Imp::~CoreLayer_Imp()
	{}

	bool CoreLayer_Imp::GetIsDrawn() const
	{
		return m_isDrawn;
	}

	void CoreLayer_Imp::SetIsDrawn(bool value)
	{
		m_isDrawn = value;
	}

}