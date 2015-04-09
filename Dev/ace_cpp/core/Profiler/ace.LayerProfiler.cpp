#include "ace.LayerProfiler.h"
#include <Utility/ace.Timer.h>

namespace ace
{
	LayerProfiler::LayerProfiler()
		: m_profiles(std::list<LayerProfile::Ptr>())
	{
	}

	LayerProfiler::~LayerProfiler()
	{
	}

	LayerProfiler* LayerProfiler::Create()
	{
		return new LayerProfiler();
	}

	void LayerProfiler::Start(astring name, int objectCount)
	{
		ACE_ASSERT(m_editing == nullptr, "編集中のプロファイルが残っています");
		m_editing = std::make_shared<LayerProfile>(name);
		m_editing->SetObjectCount(objectCount);
		m_editing->SetStartTime(ace::GetTime());
	}

	void LayerProfiler::End()
	{
		ACE_ASSERT(m_editing != nullptr, "編集中のプロファイルがありません")
		m_editing->SetEndTime(ace::GetTime());
		m_profiles.push_back(m_editing);
		m_editing = nullptr;
	}

	void LayerProfiler::Refresh()
	{
		m_profiles.clear();
	}
}