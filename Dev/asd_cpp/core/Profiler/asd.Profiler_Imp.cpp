﻿#include <algorithm>
#include <asd.common.Base.h>
#include <Utility/asd.Timer.h>
#include "asd.Profiler_Imp.h"

#if defined(_CONSOLE_GAME)
#elif defined(_WIN32)
#else
#include <sched.h>
#include <thread>
#endif

using namespace std;

namespace asd
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Profiler_Imp::Profiler_Imp()
		: m_profiles(list<Profile::Ptr>())
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
		Profile::Ptr profile = nullptr;
		for (auto& x : m_profiles)
		{
			if (x->GetID() == id)
			{
				profile = x;
			}
		}

		if (profile == nullptr)
		{
			profile = make_shared<Profile>(id);
			m_profiles.push_back(profile);
		}

		profile->GetCurrent()->SetStartTime(asd::GetTime());

#if _WIN32
		profile->GetCurrent()->SetProcessorNumber(GetCurrentProcessorNumber());
#elif defined(__APPLE__)
		// sched_getcpuがないようなので代用。よりよいものがあれば差し替えてください。
		profile->GetCurrent()->SetProcessorNumber(
			std::hash<std::thread::id>()(std::this_thread::get_id()));

#elif defined(_CONSOLE_GAME)

#else
		profile->GetCurrent()->SetProcessorNumber(sched_getcpu());
#endif
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Profiler_Imp::End(int id)
	{
		Profile::Ptr profile = nullptr;
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

		profile->GetCurrent()->SetEndTime(asd::GetTime());
		profile->Archive();
	}

#if !SWIG
	const list<Profile::Ptr>& Profiler_Imp::GetProfiles()
	{
		return m_profiles;
	}
#endif
}
