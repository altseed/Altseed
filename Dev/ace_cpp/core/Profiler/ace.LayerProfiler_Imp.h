#pragma once
#include <list>
#include "../ace.ReferenceObject.h"
#include "ace.LayerProfile.h"
#include "ace.LayerProfiler.h"

namespace ace
{
	class LayerProfiler_Imp : public LayerProfiler
	{
	private:
		LayerProfiler_Imp();
		virtual ~LayerProfiler_Imp();

		std::list<LayerProfile::Ptr> m_profiles;

	public:
		static LayerProfiler_Imp* Create();

		/**
			@brief	レイヤーのプロファイリングデータを記録する。
			@param	name		レイヤー名。
					objectCount	レイヤーに登録されているオブジェクト数。
					time		レイヤーの更新時間。
		*/
		void Record(const achar* name, int objectCount, int time);

		/**
			@brief	プロファイリングデータを消去する。更新ごとに呼び出す必要がある。
		*/
		void Refresh();

#if !SWIG
		const std::list<LayerProfile::Ptr>& GetProfiles();

		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}