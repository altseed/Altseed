#include "ace.LayerProfile.h"

namespace ace
{
	LayerProfile::LayerProfile(astring name)
		: m_name(name)
		, m_startTime(0)
		, m_endTime(0)
		, m_objectCount(0)
	{
	}

	astring LayerProfile::GetName() const
	{
		return m_name;
	}

	int LayerProfile::GetStartTime() const
	{
		return m_startTime;
	}

	void LayerProfile::SetStartTime(int value)
	{
		m_startTime = value;
	}

	int LayerProfile::GetEndTime() const
	{
		return m_endTime;
	}

	void LayerProfile::SetEndTime(int value)
	{
		m_endTime = value;
	}

	int LayerProfile::GetObjectCount() const
	{
		return m_objectCount;
	}

	void LayerProfile::SetObjectCount(int value)
	{
		m_objectCount = value;
	}
}