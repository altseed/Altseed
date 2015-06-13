#include "asd.common.Base.h"
#include "asd.LayerProfile.h"

namespace asd
{
	LayerProfile::LayerProfile(astring name)
		: m_name(name)
		, m_time(0)
		, m_objectCount(0)
	{
	}

	astring LayerProfile::GetName() const
	{
		return m_name;
	}

	int LayerProfile::GetObjectCount() const
	{
		return m_objectCount;
	}

	void LayerProfile::SetObjectCount(int value)
	{
		m_objectCount = value;
	}

	int LayerProfile::GetTime() const
	{
		return m_time;
	}

	void LayerProfile::SetTime(int value)
	{
		m_time = value;
	}

}