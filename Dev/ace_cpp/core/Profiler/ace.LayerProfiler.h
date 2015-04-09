#include <list>
#include "../ace.ReferenceObject.h"
#include "ace.LayerProfile.h"

namespace ace
{
	class LayerProfiler : public ReferenceObject
	{
	private:
		LayerProfiler();
		~LayerProfiler();

		std::list<LayerProfile::Ptr> m_profiles;
		LayerProfile::Ptr m_editing;

	public:
		static LayerProfiler* Create();

		/**
			@brief	レイヤーの更新開始時間を記録する。
			@param	name		レイヤー名。
					objectCount	レイヤーに登録されているオブジェクト数。
		*/
		void Start(astring name, int objectCount);

		/**
			@brief	レイヤーの更新終了時間を記録する。
		*/
		void End();

		/**
			@brief	プロファイリングデータを消去する。更新ごとに呼び出す必要がある。
		*/
		void Refresh();

#if !SWIG
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}