#include "ace.LayerProfiler_Imp.h"
#include <Utility/ace.Timer.h>

namespace ace
{
	LayerProfiler_Imp::LayerProfiler_Imp()
		: m_profiles(std::list<LayerProfile::Ptr>())
	{
	}

	LayerProfiler_Imp::~LayerProfiler_Imp()
	{
	}

	LayerProfiler_Imp* LayerProfiler_Imp::Create()
	{
		return new LayerProfiler_Imp();
	}

	void LayerProfiler_Imp::Record(astring name, int objectCount, int time)
	{
		auto profile = std::make_shared<LayerProfile>(name);
		profile->SetObjectCount(objectCount);
		profile->SetTime(time);
		m_profiles.push_back(profile);
	}

	void LayerProfiler_Imp::Refresh()
	{
		m_profiles.clear();
	}
}