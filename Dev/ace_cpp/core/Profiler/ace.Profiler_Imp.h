#pragma once
#include "ace.Profiler.h"
#include "ace.Profile.h"
#include <memory>
#include <list>
#include <map>

namespace ace
{
	class Profiler_Imp : public Profiler
	{
	public:
		typedef Profile* ProfilePtr;

	private:
		Profiler_Imp();
		virtual ~Profiler_Imp();

		std::list<ProfilePtr> m_profiles;

	public:
		static Profiler_Imp* Create();

		void Start(int id);
		void End(int id);

#if !SWIG
		/**
			@brief 記録されたプロファイリング結果を返す。
		*/
		const std::list<ProfilePtr>& GetProfiles();

		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}