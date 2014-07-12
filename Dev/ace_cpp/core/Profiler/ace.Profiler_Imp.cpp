#include <algorithm>
#include <ace.common.Base.h>
#include <Utility/ace.Timer.h>
#include "ace.Profiler_Imp.h"

#if !_WIN32
//#include <sched.h>
#include <thread>
#endif

using namespace std;

namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Profiler_Imp::Profiler_Imp()
		: m_profiles(list<ProfilePtr>())
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Profiler_Imp::~Profiler_Imp()
	{
	
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Profiler_Imp* Profiler_Imp::Create()
	{
		return new Profiler_Imp();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Profiler_Imp::Start(int id)
	{
		ProfilePtr profile = nullptr;
		for (auto& x : m_profiles)
		{
			if (x->GetID() == id)
			{
				profile = x;
			}
		}

		if (profile == nullptr)
		{
			profile = new Profile(id);
			m_profiles.push_back(profile);
		}

		profile->GetCurrent()->SetStartTime(ace::GetTime());

#if _WIN32
		profile->GetCurrent()->SetProcessorNumber(GetCurrentProcessorNumber());
#else
		profile->GetCurrent()->SetProcessorNumber(std::thread::hardware_concurrency());
		//profile->GetCurrent()->SetProcessorNumber(sched_getcpu());
#endif
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Profiler_Imp::End(int id)
	{
		ProfilePtr profile = nullptr;
		for (auto& x : m_profiles)
		{
			if (x->GetID() == id)
			{
				profile = x;
			}
		}

		if (profile == nullptr)
		{
#if _WIN32
			throw exception("対応するStartがありません。");
#else
			assert(0);
#endif
		}

		profile->GetCurrent()->SetEndTime(ace::GetTime());
		profile->Archive();
	}

#if !SWIG
	const list<Profiler_Imp::ProfilePtr>& Profiler_Imp::GetProfiles()
	{
		return m_profiles;
	}
#endif
}
